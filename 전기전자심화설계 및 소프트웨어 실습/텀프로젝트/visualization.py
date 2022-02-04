"""visualization.py

The BBoxVisualization class implements drawing of nice looking
bounding boxes based on object detection results.
"""


import numpy as np
import cv2


# Constants
ALPHA = 0.5
FONT = cv2.FONT_HERSHEY_PLAIN
TEXT_SCALE = 1.0
TEXT_THICKNESS = 1
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)


def gen_colors(num_colors):
    """Generate different colors.

    # Arguments
      num_colors: total number of colors/classes.

    # Output
      bgrs: a list of (B, G, R) tuples which correspond to each of
            the colors/classes.
    """
    import random
    import colorsys

    hsvs = [[float(x) / num_colors, 1., 0.7] for x in range(num_colors)]
    random.seed(1234)
    random.shuffle(hsvs)
    rgbs = list(map(lambda x: list(colorsys.hsv_to_rgb(*x)), hsvs))
    bgrs = [(int(rgb[2] * 255), int(rgb[1] * 255),  int(rgb[0] * 255))
            for rgb in rgbs]
    return bgrs


def draw_boxed_text(img, text, topleft, color):
    """Draw a transluent boxed text in white, overlayed on top of a
    colored patch surrounded by a black border. FONT, TEXT_SCALE,
    TEXT_THICKNESS and ALPHA values are constants (fixed) as defined
    on top.

    # Arguments
      img: the input image as a numpy array.
      text: the text to be drawn.
      topleft: XY coordinate of the topleft corner of the boxed text.
      color: color of the patch, i.e. background of the text.

    # Output
      img: note the original image is modified inplace.
    """
    assert img.dtype == np.uint8
    img_h, img_w, _ = img.shape
    if topleft[0] >= img_w or topleft[1] >= img_h:
        return img
    margin = 3
    size = cv2.getTextSize(text, FONT, TEXT_SCALE, TEXT_THICKNESS)
    w = size[0][0] + margin * 2
    h = size[0][1] + margin * 2
    # the patch is used to draw boxed text
    patch = np.zeros((h, w, 3), dtype=np.uint8)
    patch[...] = color
    cv2.putText(patch, text, (margin+1, h-margin-2), FONT, TEXT_SCALE,
                WHITE, thickness=TEXT_THICKNESS, lineType=cv2.LINE_8)
    cv2.rectangle(patch, (0, 0), (w-1, h-1), BLACK, thickness=1)
    w = min(w, img_w - topleft[0])  # clip overlay at image boundary
    h = min(h, img_h - topleft[1])
    # Overlay the boxed text onto region of interest (roi) in img
    roi = img[topleft[1]:topleft[1]+h, topleft[0]:topleft[0]+w, :]
    cv2.addWeighted(patch[0:h, 0:w, :], ALPHA, roi, 1 - ALPHA, 0, roi)
    return img


class BBoxVisualization():                                             # 해당 클래스 내의 함수들을 추가하여 여러 메뉴를 구현하였음
    """BBoxVisualization class implements nice drawing of boudning boxes.

    # Arguments
      cls_dict: a dictionary used to translate class id to its name.
    """

    def __init__(self, cls_dict):
        self.cls_dict = cls_dict
        self.colors = gen_colors(len(cls_dict))


    def draw_bboxes1(self, img, boxes, confs, clss):                   # 1번 메뉴 선택(이미지 내 특정 물체 찾기)시 불러오는 함수
        """Draw detected bounding boxes on the original image."""
        dict = {'person': 0, 'bicycle': 1, 'car': 2, 'motorbike': 3, 'aeroplane': 4, 'bus': 5, 'train': 6, 'truck': 7,
                'boat': 8, 'traffic light': 9, 'fire hydrant': 10, 'stop sign': 11, 'parking meter': 12, 'bench': 13,
                'bird': 14, 'cat': 15, 'dog': 16, 'horse': 17, 'sheep': 18, 'cow': 19, 'elephant': 20, 'bear': 21,
                'zebra': 22, 'giraffe': 23, 'backpack': 24, 'unbrella': 25, 'handbag': 26, 'tie': 27, 'suitcase': 28,
                'frisbee': 29, 'skis': 30, 'snowboard': 31, 'sports ball': 32, 'kite': 33, 'baseball bat': 34,
                'baseball glove': 35, 'skateboard': 36, 'surfboard': 37, 'tennis racket': 38, 'bottle': 39,
                'wine glass': 40, 'cup': 41, 'fork': 42, 'knife': 43, 'spoon': 44, 'bowl': 45, 'banana': 46,
                'apple': 47, 'sandwich': 48, 'orange': 49, 'broccoli': 50, 'carrot': 51, 'hot dog': 52, 'pizza': 53,
                'donut': 54, 'cake': 55, 'chair': 56, 'sofa': 57, 'pottedplant': 58, 'bed': 59, 'diningtable': 60,
                'toilet': 61, 'tvmonitor': 62, 'laptop': 63, 'mouse': 64, 'remote': 65, 'keyboard': 66,
                'cell phone': 67, 'microwave': 68, 'oven': 69, 'toaster': 70, 'sink': 71, 'refrigerator': 72,
                'book': 73, 'clock': 74, 'vase': 75, 'scissors': 76, 'teddy bear': 77, 'hair drier': 78,
                'toothbrush': 79}              # 입력으로 클래스의 이름을 받아오면 해당 입력값을 정수로 변환하기 위한 dictionary

        tmp = dict[(input('Input class : '))]    # 찾고자 하는 클래스를 문자열로 입력하면 dict을 사용하여 정수값으로 반환, tmp에 저장
        for bb, cf, cl in zip(boxes, confs, clss):                              # 검출한 물체들을 바탕으로 for문 반복
            cl = int(cl)
            if tmp == cl:                                                       # 만약 검출한 물체와 찾으려는 물체가 같다면
                x_min, y_min, x_max, y_max = bb[0], bb[1], bb[2], bb[3]         # bbox를 그린다
                color = self.colors[cl]
                cv2.rectangle(img, (x_min, y_min), (x_max, y_max), color, 2)
                txt_loc = (max(x_min + 2, 0), max(y_min + 2, 0))
                cls_name = self.cls_dict.get(cl, 'CLS{}'.format(cl))
                txt = '{} {:.2f}'.format(cls_name, cf)
                img = draw_boxed_text(img, txt, txt_loc, color)

                                                                                # 검출한 물체와 찾으려는 물체가 다르다면
                                                                                # bbox를 그리지 않는다.
        return img

    def draw_bboxes2(self, img, boxes, confs, clss):              # 2번 메뉴 선택(이미지 내 물체들의 가격파악)시 불러오는 함수
        """Draw detected bounding boxes on the original image."""
        price = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100000, 15000, 150000, 10000,
                 200000, 0, 0, 0, 5000, 5000, 220000, 20000, 50000, 330000, 300000, 15000, 30000, 20000, 5000, 5000,
                 5000, 20000, 8000, 1000, 7000, 1000, 500, 500, 3000, 20000, 3000, 30000, 50000, 500000, 40000, 400000,
                 80000, 0, 2000000, 1700000, 30000, 10000, 40000, 1500000, 40000, 250000, 30000, 0, 1600000, 10000,
                 70000, 20000, 5000, 40000, 30000, 2000]        # 0~79개 클래스의 가격표 리스트, 0은 가격을 매길 수 없는 물건들

        cnt = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0]                    # 0~79개 클래스가 이미지 내 몇개가 있는지 세기위한 cnt 리스트

        tag = {0: 'person', 1: 'bicycle', 2: 'car', 3: 'motorbike', 4: 'aeroplane', 5: 'bus', 6: 'train', 7: 'truck',
                8: 'boat', 9: 'traffic light', 10: 'fire hydrant', 11: 'stop sign', 12: 'parking meter', 13: 'bench',
                14: 'bird', 15: 'cat', 16: 'dog', 17: 'horse', 18: 'sheep', 19: 'cow', 20: 'elephant', 21: 'bear',
                22: 'zebra', 23: 'giraffe', 24: 'backpack', 25: 'umbrella', 26: 'handbag', 27: 'tie', 28: 'suitcase',
                29: 'frisbee', 30: 'skis', 31: 'snowboard', 32: 'sports ball', 33: 'kite', 34: 'baseball bat',
                35: 'baseball glove', 36: 'skateboard', 37: 'surfboard', 38: 'tennis racket', 39: 'bottle',
                40: 'wine glass', 41: 'cup', 42: 'fork', 43: 'knife', 44: 'spoon', 45: 'bowl', 46: 'banana',
                47: 'apple', 48: 'sandwich', 49: 'orange', 50: 'broccoli', 51: 'carrot', 52: 'hot dog', 53: 'pizza',
                54: 'donut', 55: 'cake', 56: 'chair', 57: 'sofa', 58: 'pottedplant', 59: 'bed', 60: 'diningtable',
                61: 'toilet', 62: 'tvmonitor', 63: 'laptop', 64: 'mouse', 65: 'remote', 66: 'keyboard',
                67: 'cell phone', 68: 'microwave', 69: 'oven', 70: 'toaster', 71: 'sink', 72: 'refrigerator',
                73: 'book', 74: 'clock', 75: 'vase', 76: 'scissors', 77: 'teddy bear', 78: 'hair drier',
                79: 'toothbrush'}                            # 클래스를 나타내는 0~79의 정수를 문자열로 변환하기위한 dictionary

        for bb, cf, cl in zip(boxes, confs, clss):          # 검출한 물체들을 바탕으로 for문 반복
            cl = int(cl)
            cnt[cl] += 1;                                   # 검출한 물체에 해당하는 순서의 cnt리스트에 1씩 더해 물체 개수를 셈

            x_min, y_min, x_max, y_max = bb[0], bb[1], bb[2], bb[3]         # bbox 그리기
            color = self.colors[cl]
            cv2.rectangle(img, (x_min, y_min), (x_max, y_max), color, 2)
            txt_loc = (max(x_min + 2, 0), max(y_min + 2, 0))
            cls_name = self.cls_dict.get(cl, 'CLS{}'.format(cl))
            txt = '{} {:.2f}'.format(cls_name, cf)
            img = draw_boxed_text(img, txt, txt_loc, color)

        sum = 0                                                            # 총합을 나타낼 sum 변수를 0으로 초기화
        for i in range(80):                                                # 0~79개의 모든 클래스에 대해 반복
            sum += cnt[i] * price[i]                                       # sum 변수에 클래스 가격과 클래스 개수를 곱하여 누계
            if (cnt[i] > 0) & (price[i]!=0):                 # 물체가 한번 이상 검출되었고, 가격을 매길 수 있는 물체라면 if문 실행
                print(tag[i]+' : '+str(cnt[i])+' * '+ str(price[i])+'won')       # 각 물체의 이름과 개수, 가격을 출력
        print("\ntotal : {}won.".format(sum))                               # 총 가격을 출력
        return img

    def draw_bboxes3(self, img, boxes, confs, clss):                        # 3번, 4번 메뉴 선택(이미지 내 물체들을 탐지한 후 폴더별로 분류)시 불러오는 함수
        """Draw detected bounding boxes on the original image."""
        for bb, cf, cl in zip(boxes, confs, clss):                          # 검출한 물체들을 바탕으로 for문 반복
            cl = int(cl)
            x_min, y_min, x_max, y_max = bb[0], bb[1], bb[2], bb[3]
            color = self.colors[cl]
            cv2.rectangle(img, (x_min, y_min), (x_max, y_max), color, 2)
            txt_loc = (max(x_min+2, 0), max(y_min+2, 0))
            cls_name = self.cls_dict.get(cl, 'CLS{}'.format(cl))
            txt = '{} {:.2f}'.format(cls_name, cf)
            img = draw_boxed_text(img, txt, txt_loc, color)
        return img

