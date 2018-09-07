import numpy as np
import image_pb2
import cv2
cv_img=cv2.imread("/media/lee/workspace/GitWorkSpace/lpc-iot/GRPC/image_msg/image.jpg")
bin_img=cv_img.tobytes()

img_pb=image_pb2.image()
img_pb.width=cv_img.shape[1]
img_pb.height=cv_img.shape[0]
img_pb.channel=cv_img.shape[2]
img_pb.data=bin_img
data=img_pb.SerializeToString()
with open("data.txt",'wb') as f:
    f.write(data)

print(img_pb.ByteSize())
img_pb1=image_pb2.image()
img_pb1.ParseFromString(data)
print(img_pb1.width)
print(img_pb1.height)
img3=np.frombuffer(img_pb1.data,dtype=np.uint8)
img3.resize((img_pb1.height,img_pb1.width,img_pb1.channel))
cv2.imshow("",img3)
cv2.waitKey(0)