import msgpackrpc
import numpy as np
# import  cv2
# import  time
import json

class ImageProcess(object):
    def inference(self,data):
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
        result={"model":"yolo","objects":[]}
        result["objects"].append({"class":"mouse","xmin":200,"ymin":200,"width":100,"height":100})
        result["objects"].append({"class":"mouse","xmin":150,"ymin":150,"width":50,"height":50})
        print(result)
        return json.dumps(result)
        

server=msgpackrpc.Server(ImageProcess())
server.listen(msgpackrpc.Address("127.0.0.1",8800))
server.start()
