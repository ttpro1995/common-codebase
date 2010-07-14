#!/usr/bin/env python
#coding=utf-8

from win32com import client as wc

word = wc.Dispatch('Word.Application')
doc = word.Documents.Open(r'D:\documents\Template\1620-00100-00_FunctionalSpecification.doc')
doc.SaveAs(r'd:\test.pdf', 17)
doc.Close()
word.Quit()
