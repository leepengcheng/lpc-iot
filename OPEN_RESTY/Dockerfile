
FROM openresty/openresty:latest
LABEL Name=myOpenresty Version=0.0.1
ENV PATH=$PATH:/usr/local/openresty/bin
COPY /conf/nginx.conf /usr/local/openresty/nginx/conf/nginx.conf
COPY /logs /usr/local/openresty/nginx/logs
COPY /html /usr/local/openresty/nginx/html
COPY /lua /usr/local/openresty/nginx/lua

VOLUME [ "/db" ]