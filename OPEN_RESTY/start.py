#coding:utf-8
import subprocess
import os
# HostDir="D:/MyGit/lpc-iot/OPEN_RESTY"
HostDir=os.path.dirname(__file__).replace("\\","/")
DockerDir="/usr/local/openresty/nginx"
cmd='''docker run -it \
        --name myopenresty \
        -p 80:80 \
        -v  {HostDir}/conf/nginx.conf:{DockerDir}/conf/nginx.conf:rw\
        -v  {HostDir}/logs:{DockerDir}/logs:rw\
        -v  {HostDir}/html:{DockerDir}/html:rw\
        -v  {HostDir}/lua:{DockerDir}/lua:rw\
        openresty/openresty:latest'''.format(HostDir=HostDir,DockerDir=DockerDir)

subprocess.Popen(cmd,shell=True)