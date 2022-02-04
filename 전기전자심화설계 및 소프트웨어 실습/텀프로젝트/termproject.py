"""trt_yolo.py

This script demonstrates how to do real-time object detection with
TensorRT optimized YOLO engine.
"""


import os
import time
import argparse

import cv2
import pycuda.autoinit  # This is needed for initializing CUDA driver

from utils.yolo_classes_hh import get_cls_dict
from utils.camera import add_camera_args, Camera
from utils.display import open_window, set_display, show_fps
from utils.visualization import BBoxVisualization
from utils.yolo_with_plugins import TrtYOLO


WINDOW_NAME = 'TrtYOLODemo'


def parse_args():
    """Parse input arguments."""
    desc = ('Capture and display live camera video, while doing '
            'real-time object detection with TensorRT optimized '
            'YOLO model on Jetson')
    parser = argparse.ArgumentParser(description=desc)
    parser = add_camera_args(parser)
    parser.add_argument(
        '-c', '--category_num', type=int, default=80,
        help='number of object categories [80]')
    parser.add_argument(
        '-m', '--model', type=str, required=True,
        help=('[yolov3-tiny|yolov3|yolov3-spp|yolov4-tiny|yolov4|'
              'yolov4-csp|yolov4x-mish]-[{dimension}], where '
              '{dimension} could be either a single number (e.g. '
              '288, 416, 608) or 2 numbers, WxH (e.g. 416x256)'))
    parser.add_argument(
        '-l', '--letter_box', action='store_true',
        help='inference with letterboxed image [False]')
    args = parser.parse_args()
    return args


def loop_and_detect(cam, trt_yolo, conf_th, vis):                               # 본 함수를 수정하여 여러 동작들을 구현 
    """Continuously capture images from camera and do object detection.

    # Arguments
      cam: the camera instance (video source).
      trt_yolo: the TRT YOLO object detector instance.
      conf_th: confidence/score threshold for object detection.
      vis: for visualization.
    """
    full_scrn = False
    fps = 0.0
    tic = time.time()
    dict = {0: 'person', 1: 'bicycle', 2: 'car', 3: 'motorbike', 4: 'aeroplane', 5: 'bus', 6: 'train', 7: 'truck',
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
            79: 'toothbrush'}           # 해당 함수내에서 0~79의 정수(클래스넘버)를 문자로 반환하기 위한 dictionary

    while True:                                                         # 동작 반복, 5번을 입력하여 프로그램 종료
        print("\nSelect_mode \n1. Detect class in image\n2. Image price calculator\n3. My album")
        print("4. Finding the moment of the video\n5. Program exit\n")          # 메뉴 출력
        print("----> ", end=' ')
        mode = int(input())                                                     # 메뉴 선택
        if mode==1:                                                     # 1번 메뉴 선택, 이미지 내 특정 클래스만을 탐지하여 bbox를 그린다.
            img = cv2.imread('input1.jpg', cv2.IMREAD_COLOR)            # 이미지 불러오기      
            if img is None:
                print("wrong!")                                         # 이미지 이름을 잘못 입력 시, 에러코드 출력
            boxes, confs, clss = trt_yolo.detect(img, conf_th)

            tmp = 1500
            print("Object Detect list: ", end=' ')                      # 해당 이미지 내 검출한 클래스들의 정보를 모두 출력

            for i in clss:                                              # 검출한 클래스 리스트들에 대하여 반복
                if tmp == i:                                            # 클래스 정보 출력에서 중복을 막기 위함
                    pass
                else:                                                   # 클래스 정보 출력, 클래스넘버를 클래스 이름으로 반환
                    print(dict[i], end=' ')
                    tmp = i
            print(" ")
            img = vis.draw_bboxes1(img, boxes, confs, clss)             # draw_bboxes1 함수 호출, 해당 함수에서 검출하려는 특정 클래스를 입력한다.
            cv2.imwrite('/home/vlsi4/project/tensorrt_demos/output/output1.jpg', img)      # 해당 경로에 output1 이름으로 특정 클래스만을 bbox처리한 이미지 저장
            
        elif mode==2:                                                   # 2번 메뉴 선택, 이미지내 클래스들을 검출하여 각각의 가격들을 계산한다.

            img = cv2.imread('input2.jpg', cv2.IMREAD_COLOR)
            if img is None:
                print("wrong!")
            boxes, confs, clss = trt_yolo.detect(img, conf_th)

            tmp = 1500
            print("Object Detect list: ", end=' ')

            for i in clss:
                if tmp == i:
                    pass
                else:
                    print(dict[i], end=' ')
                    tmp = i
            print(" ")                                              # 1번 메뉴의 내용과 동일
            img = vis.draw_bboxes2(img, boxes, confs, clss)         # draw_bboxes2 함수 호출, 해당 함수에서 가격 계산과 가격 정보들을 출력한다.
            cv2.imwrite('/home/vlsi4/project/tensorrt_demos/output/output2.jpg', img)       # 이미지 저장

        elif mode==3:                                                # 3번 메뉴 선택, 이미지내 클래스들을 검출한뒤 각각의 클래스폴더에 이미지를 저장한다.
        #if cv2.getWindowProperty(WINDOW_NAME, 0) < 0:
        #    break
            img = cv2.imread('input3.jpg', cv2.IMREAD_COLOR)
            if img is None:
                print("wrong!")
            boxes, confs, clss = trt_yolo.detect(img, conf_th)


            tmp=1500
            print("Object Detect list: ", end=' ')
            img = vis.draw_bboxes3(img, boxes, confs, clss)             # draw_bboxes3 함수 호출, 해당 함수에서 클래스들에 대해 bbox처리
            for i in clss:
                if tmp==i:
                    pass
                else:
                    print(dict[i], end=' ')
                    cv2.imwrite('/home/vlsi4/project/tensorrt_demos/sort/sort_'+dict[i]+'/output.jpg', img)  # 검출한 클래스 별로 각각의 폴더에 저장한다.
                    tmp=i
            print(" ")                                                          # 주석 처리한 부분을 제외하고는 1번메뉴와 동일

        elif mode==4:                                              # 4번 메뉴 선택 시, 동영상내의 특정 클래스가 몇 초에 있는지 검색
            cap = cv2.VideoCapture('input4.mp4')
            fps = 24.0                                          # input으로 사용하는 동영상의 fps와 맞춰줘야 한다.
            fourcc = cv2.VideoWriter_fourcc(*'mp4v')
            width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
            height = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
            size = (int(width), int(height))
            out = cv2.VideoWriter('output4.mp4', fourcc, fps, size)     # 동영상을 처리하기 위한 여러가지 변수들을 선언
            timer = 0                                                   # 동영상 내 초를 세기위한 timer
            find = {'person': 0, 'bicycle': 1, 'car': 2, 'motorbike': 3, 'aeroplane': 4, 'bus': 5, 'train': 6,
                    'truck': 7,
                    'boat': 8, 'traffic light': 9, 'fire hydrant': 10, 'stop sign': 11, 'parking meter': 12,
                    'bench': 13,
                    'bird': 14, 'cat': 15, 'dog': 16, 'horse': 17, 'sheep': 18, 'cow': 19, 'elephant': 20, 'bear': 21,
                    'zebra': 22, 'giraffe': 23, 'backpack': 24, 'unbrella': 25, 'handbag': 26, 'tie': 27,
                    'suitcase': 28,
                    'frisbee': 29, 'skis': 30, 'snowboard': 31, 'sports ball': 32, 'kite': 33, 'baseball bat': 34,
                    'baseball glove': 35, 'skateboard': 36, 'surfboard': 37, 'tennis racket': 38, 'bottle': 39,
                    'wine glass': 40, 'cup': 41, 'fork': 42, 'knife': 43, 'spoon': 44, 'bowl': 45, 'banana': 46,
                    'apple': 47, 'sandwich': 48, 'orange': 49, 'broccoli': 50, 'carrot': 51, 'hot dog': 52, 'pizza': 53,
                    'donut': 54, 'cake': 55, 'chair': 56, 'sofa': 57, 'pottedplant': 58, 'bed': 59, 'diningtable': 60,
                    'toilet': 61, 'tvmonitor': 62, 'laptop': 63, 'mouse': 64, 'remote': 65, 'keyboard': 66,
                    'cell phone': 67, 'microwave': 68, 'oven': 69, 'toaster': 70, 'sink': 71, 'refrigerator': 72,
                    'book': 73, 'clock': 74, 'vase': 75, 'scissors': 76, 'teddy bear': 77, 'hair drier': 78,
                    'toothbrush': 79}           # 클래스 이름 (문자열)을 입력하면 정수로 변환하기 위한 dictionary

            print("Enter what you want to find  ---> ", end=' ')
            a = find[input()]                   # 동영상 내 찾으려는 클래스 이름을 입력받고 이를 정수로 변환한 값을 a변수에 저장
            while (cap.isOpened()):                 # 동영상이 처리되는 동안 반복
                ret, frame = cap.read()
                timer += 1                          # timer 변수 1씩 증가
                if ret:
                    out.write(frame)
                else:                               # 만약 동영상 처리가 끝나면
                    print("finish video processing!! (video length : {}sec)".format(int(timer/fps)))    # 동영상 길이와 함께 동영상 처리가 끝났음을 출력
                    break

                if timer % int(fps) == 0:                                   # timer가 fps의 정수배이면 (=1초 단위로)
                    boxes, confs, clss = trt_yolo.detect(frame, conf_th)    # detect 함수 호출하여 클래스 정보를 받아온다.
                    for i in clss:
                        if a == i:                                # 만약 찾는 클래스가 현재 프레임에 있다면
                            print("{}sec ---> here it is !".format(timer/int(fps)))  # 동영상 내 해당 시간에 찾는 클래스가 있음을 출력
                            break

                    frame = vis.draw_bboxes3(frame, boxes, confs, clss)     # 초단위로 frame의 bbox를 그린다
                    cv2.imwrite('/home/vlsi4/project/tensorrt_demos/output/video_finding/'+str(timer/int(fps))+'.jpg', frame)   # 초단위로 frame을 저장한다.


            cap.release()
            out.release()               # 동영상 처리 종료


        elif mode==5:                   # 5번 메뉴 선택 시 반복문을 탈출하여 프로그램을 종료한다.
            for i in range(80):
                if os.path.isfile('/home/vlsi4/project/tensorrt_demos/sort/sort_' + dict[i] + '/output.jpg'):
                    os.remove('/home/vlsi4/project/tensorrt_demos/sort/sort_' + dict[i] + '/output.jpg')
            break

        else:                           # 만약 잘못된 메뉴 선택 시 재안내
            print("Enter the wrong number ! Please Retry ")
            

def main():
    args = parse_args()
    if args.category_num <= 0:
        raise SystemExit('ERROR: bad category_num (%d)!' % args.category_num)
    if not os.path.isfile('yolo/%s.trt' % args.model):
        raise SystemExit('ERROR: file (yolo/%s.trt) not found!' % args.model)

    cam = Camera(args)
    if not cam.isOpened():
        raise SystemExit('ERROR: failed to open camera!')

    cls_dict = get_cls_dict(args.category_num)
    vis = BBoxVisualization(cls_dict)
    trt_yolo = TrtYOLO(args.model, args.category_num, args.letter_box)

    #open_window(
    #    WINDOW_NAME, 'Camera TensorRT YOLO Demo',
    #    cam.img_width, cam.img_height)
    loop_and_detect(cam, trt_yolo, conf_th=0.3, vis=vis)

    #cam.release()
    #cv2.destroyAllWindows()

if __name__ == '__main__':
    main()

