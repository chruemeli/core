name = "Chruemeli"
version = "0.1.0"
author = "Simon Erhardt <hello@rootlogin.ch>"
title = "An IOT-Framework for ESP8266"
description = "An IOT-Framework for ESP8266"
category = "Other"
url = "https://github.com/chrootlogin/"
depends = "CRC32, ArduinoJson"

with open('library.properties', 'w') as f:
    f.write("name=%s\n" % name)
    f.write("version=%s\n" % version)
    f.write("author=%s\n" % author)
    f.write("maintainer=%s\n" % author)
    f.write("sentence=%s\n" % title)
    f.write("paragraph=%s\n" % description)
    f.write("url=%s\n" % url)
    f.write("category=%s\n" % category)
    f.write("depends=%s\n" % depends)
    f.write("architecture=*\n")