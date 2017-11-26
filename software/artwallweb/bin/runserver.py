#!/usr/bin/env python
# -*- coding: utf-8 -*-
# artwall (c) Kent Hutson

from artwall.wsgi import app
app.run(port=app.config['PORT'])
