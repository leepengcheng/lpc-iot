import msgpackrpc
import numpy as np
import  cv2
import  time

cv2.namedWindow("RPC")
class ImageProcess(object):
    def get_halcon_image(self,data):
        w=data[b'width']
        h=data[b'height']
        img=np.zeros((h,w,3),dtype=np.uint8)
        r=np.frombuffer(data[b'r'],dtype=np.uint8)
        g=np.frombuffer(data[b'g'],dtype=np.uint8)
        b=np.frombuffer(data[b'b'],dtype=np.uint8)
        r.resize((h,w))
        g.resize((h,w))
        b.resize((h,w))
        img[:,:,0]=b
        img[:,:,1]=g
        img[:,:,2]=r
        cv2.imshow("RPC",img)
        cv2.waitKey(100)



server=msgpackrpc.Server(ImageProcess())
server.listen(msgpackrpc.Address("0.0.0.0",8800))
server.start()
