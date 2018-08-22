local redis = require "resty.redis"
local red = redis:new()
red:set_timeout(1000) -- 1 sec
-- or connect to a unix domain socket file listened
-- by a redis server:
-- local ok, err = red:connect("unix:/path/to/redis.sock")
local ok, err = red:connect("redis_db", 6379)
if not ok then
    ngx.say("failed to connect: ", err)
    return
end
local res, err = red:get("cat")
if not res then
    ngx.say("failed to get cat: ", err)
else
    ngx.say("cat: ", res)
end
ok, err = red:set("cat", "an animal too")
if not ok then
    ngx.say("failed to set cat: ", err)
    return
end
ngx.say("set result: ", ok)
-- put it into the connection pool of size 100,
-- with 10 seconds max idle time
local ok, err = red:set_keepalive(10000, 100)
    if not ok then
    ngx.say("failed to set keepalive: ", err)
    return
end