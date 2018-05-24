#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# Created on 2017-02-04 15:29:34
# Project: my6v

from pyspider.libs.base_handler import *


class Handler(BaseHandler):
    crawl_config = {
    }

    @every(minutes=24 * 60)
    def on_start(self):
        self.crawl('http://www.6vhao.net/', callback=self.index_page)

    @config(age=10 * 24 * 60 * 60)
    def index_page(self, response):
        for each in list(response.doc('div.menutv > ul >li > a').items())[1:3]:
            self.crawl(each.attr.href, callback=self.category_pages)
            
    @config(age=10 * 24 * 60 * 60)
    def category_pages(self, response):
        items=list(response.doc('div.pagebox:first > a[href]').items())
        last_url=items[-1].attr.href
        sum_page=int(last_url.split("_")[1].split(".")[0])
        self.crawl(response.url, callback=self.detail_page)
        for i in range(2,4):
            url="%sindex_%s.html"%(response.url,i)
            self.crawl(url, callback=self.category_singlepage)
            
    @config(age=10 * 24 * 60 * 60)
    def category_singlepage(self, response):
        items=response.doc('div.listInfo > h3 > a').items()
        for each in items:
            self.crawl(each.attr.href, callback=self.detail_page)  

    
    @config(priority=2)
    def detail_page(self, response):
        text=response.doc("div#text").text()
        downloads=[x.attr.href for x in response.doc("div#text  a").items()]
        return {
            "url": response.url,
            "title": response.doc('title').text(),
            "text":response.doc("div#text").text(),
            "downloads":downloads
        }

#     @catch_status_code_error  
#     def callback(self, response):
#         with open(r"d:\error.txt") as f:
#             f.write(response.url)


