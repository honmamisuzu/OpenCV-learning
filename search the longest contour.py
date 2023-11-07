import cv2
import numpy as np

# 初始化摄像头
cap = cv2.VideoCapture(0)

threshold=1

while True:
    # 读取摄像头画面
    ret, frame = cap.read()

    # 进行边缘检测
    edged = cv2.Canny(frame, threshold, 100*threshold)
    # 查找轮廓
    contours, _ = cv2.findContours(edged, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # 查找最长轮廓
    max_contour = max(contours, key=cv2.contourArea)
    # 画出最长轮廓
    cv2.drawContours(frame, [max_contour], 0, (255,0,0), 3)
    # 显示画面
    cv2.imshow('Longest Contour', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
# 释放摄像头并关闭窗口
cap.release()
cv2.destroyAllWindows()
