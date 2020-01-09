import os
import sys
import cv2
import numpy as np

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from datetime import datetime
import math
import time
from data import inputs
import numpy as np
import tensorflow as tf
from model import select_model, get_checkpoint
from utils import *
import os
import json
import csv

RESIZE_FINAL = 227
GENDER_LIST =['M','F']
AGE_LIST = ['(0, 2)','(4, 6)','(8, 12)','(15, 20)','(25, 32)','(38, 43)','(48, 53)','(60, 100)']
MAX_BATCH_SZ = 128

class AgenderDetector:
    def __init__(self, image):
        config = tf.ConfigProto(allow_soft_placement=True)
        self._sess = tf.Session(config=config)

        image_tf = tf.image.decode_png(image, channels=3)
        self._png_to_jpeg = tf.image.encode_jpeg(image_tf, format='rgb', quality=100)







