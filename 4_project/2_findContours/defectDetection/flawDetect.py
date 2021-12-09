import cv2 as cv

image = cv.imread("F://pj_python//1_jihua//images_prj//flaw.png")
gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
binary = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, 7, 2)
e1 = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))
temp1 = cv.morphologyEx(binary, cv.MORPH_OPEN, e1, iterations=1)
cv.imshow("binary", binary)
cv.imshow("temp1", temp1)
contours, hireachy = cv.findContours(temp1, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
r = cv.drawContours(image, contours, -1, (0, 0, 255), 2)
cv.imshow("result", r)
cv.waitKey()
