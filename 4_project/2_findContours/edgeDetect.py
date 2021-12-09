import cv2
import numpy as np

# 读取图片, 并准换成灰度图
img = cv2.imread("F://pj_python//1_jihua//images_prj//image_roi.png")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# -------------------------------------
# sobel
x = cv2.Sobel(img, cv2.CV_16S, 1, 0, 5)
y = cv2.Sobel(img, cv2.CV_16S, 0, 1, 5)

absX = cv2.convertScaleAbs(x)  # 转回uint8
absY = cv2.convertScaleAbs(y)
dst = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
cv2.imshow("absX", absX)
cv2.imshow("absY", absY)
cv2.imshow("Result", dst)
cv2.waitKey()

# -------------------------------------
# laplacian算子
# 高斯滤波器 (3 X 3)
gray = cv2.GaussianBlur(gray, (3, 3), sigmaX=0.01)
# Laplacian 算子
laplacian = cv2.Laplacian(gray, cv2.CV_64F)
# 取绝对值
laplacian = cv2.convertScaleAbs(laplacian)
# 展示图片
cv2.imshow("laplacian", laplacian)
cv2.waitKey(0)
cv2.destroyAllWindows()

# -------------------------------------
# Scharr 算子
scharr_x = cv2.Scharr(img, cv2.CV_64F, 1, 0)
scharr_y = cv2.Scharr(img, cv2.CV_64F, 0, 1)

# 取绝对值
scharr_x = cv2.convertScaleAbs(scharr_x)
scharr_y = cv2.convertScaleAbs(scharr_y)
# 融合
scharr_xy = cv2.addWeighted(scharr_x, 0.5, scharr_y, 0.5, 0)
# 展示图片
cv2.imshow("scharr_xy", scharr_xy)
cv2.waitKey(0)
cv2.destroyAllWindows()

# -------------------------------------
# Canny边缘检测
out1 = cv2.Canny(img, 50, 150)
out2 = cv2.Canny(img, 100, 150)
# 合并
canny = np.hstack((out1, out2))
# 展示图片
cv2.imshow("canny", canny)
cv2.waitKey(0)
cv2.destroyAllWindows()
