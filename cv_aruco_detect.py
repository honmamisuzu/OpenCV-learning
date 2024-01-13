import cv2
import cv2.aruco as aruco

# 初始化摄像头
cap = cv2.VideoCapture(1)

# 设置 ArUco 字典
aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)

# 创建 ArUco 参数
parameters = cv2.aruco.DetectorParameters_create()
while True:
    # 读取摄像头帧
    ret, frame = cap.read()

    # 将帧转换为灰度图
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 检测 ArUco 标记
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

    # 在图像上绘制检测到的标记
    if ids is not None:
        cv2.aruco.drawDetectedMarkers(frame, corners)

    # 显示帧
    cv2.imshow('ArUco Marker Detection', frame)

    # 检测键盘按键，如果是ESC键则退出循环
    if cv2.waitKey(1) & 0xFF == 27:
        break

# 释放摄像头和关闭窗口
cap.release()
cv2.destroyAllWindows()
