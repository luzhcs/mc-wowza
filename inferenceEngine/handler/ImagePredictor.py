# Import required modules
import time
import cv2 as cv


MODEL_MEAN_VALUES = (78.4263377603, 87.7689143744, 114.895847746)
ageList = ['(0-2)', '(4-6)', '(8-12)', '(15-20)', '(25-32)', '(38-43)', '(48-53)', '(60-100)']
genderList = ['Male', 'Female']

def getFaceBox(net, frame, conf_threshold=0.7):
    frame_opencv_dnn = frame.copy()
    frame_height = frame_opencv_dnn.shape[0]
    frame_width = frame_opencv_dnn.shape[1]
    blob = cv.dnn.blobFromImage(frame_opencv_dnn, 1.0, (300, 300), [104, 117, 123], True, False)

    net.setInput(blob)
    detections = net.forward()
    bboxes = []
    for i in range(detections.shape[2]):
        confidence = detections[0, 0, i, 2]
        if confidence > conf_threshold:
            x1 = int(detections[0, 0, i, 3] * frame_width)
            y1 = int(detections[0, 0, i, 4] * frame_height)
            x2 = int(detections[0, 0, i, 5] * frame_width)
            y2 = int(detections[0, 0, i, 6] * frame_height)
            bboxes.append([x1, y1, x2, y2])
            cv.rectangle(
                frame_opencv_dnn, (x1, y1), (x2, y2), (0, 255, 0), int(round(frame_height/150)), 8
            )

    return frame_opencv_dnn, bboxes

class ImagePredictor:
    def __init__(self):
        faceProto = "models/opencv_face_detector.pbtxt"
        faceModel = "models/opencv_face_detector_uint8.pb"

        ageProto = "models/age_deploy.prototxt"
        ageModel = "models/age_net.caffemodel"

        genderProto = "models/gender_deploy.prototxt"
        genderModel = "models/gender_net.caffemodel"

        # Load network
        self.ageNet=cv.dnn.readNet(ageModel, ageProto)
        self.genderNet=cv.dnn.readNet(genderModel, genderProto)
        self.faceNet=cv.dnn.readNet(faceModel, faceProto)       

    def read_image(self, path):
        try:
            cap = cv.VideoCapture(path)
            r, frame = cap.read()
            if r:
                return frame
                
        except Exception as e:
            print (str(e))
        
        return None

    def predict(self, frame):
        padding = 20
        # Read frame
        t = time.time()
        ret = []
        frameFace, bboxes = getFaceBox(self.faceNet, frame)
        if not bboxes:
            print("No face Detected, Checking next frame")
        else:

            for bbox in bboxes:
                # print(bbox)
                face = frame[
                    max( 0, bbox[1]-padding):min(bbox[3]+padding,
                        frame.shape[0]-1),max(0,bbox[0]-padding
                    ):min(bbox[2]+padding, frame.shape[1]-1)]

                blob = cv.dnn.blobFromImage(face, 1.0, (227, 227), MODEL_MEAN_VALUES, swapRB=False)
                self.genderNet.setInput(blob)
                genderPreds = self.genderNet.forward()
                gender = genderList[genderPreds[0].argmax()]
                print("Gender Output : {}".format(genderPreds))
                print("Gender : {}, conf = {:.3f}".format(gender, genderPreds[0].max()))

                self.ageNet.setInput(blob)
                agePreds = self.ageNet.forward()
                age = ageList[agePreds[0].argmax()]
                print("Age Output : {}".format(agePreds))
                print("Age : {}, conf = {:.3f}".format(age, agePreds[0].max()))
                

                label = "{},{}".format(gender, age)
                ret.append({"age": age, "gender": gender})
            
            cv.putText(frameFace, label, (bbox[0], bbox[1]-10), cv.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2, cv.LINE_AA)
            cv.imwrite("age-gender.jpg", frameFace)
        print("time : {:.3f}".format(time.time() - t))

        return ret