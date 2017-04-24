# -*- coding: utf-8 -*-

from pyparsing import Word, OneOrMore

inFilename = 'out1.txt'
FIN = open(inFilename, 'r')
TEXT = FIN.read()

myDigits = '0123456789'
eng_alphas = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
rus_alphas = 'йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ'
punctuation = '.,:;'
myPrintables = myDigits + eng_alphas + rus_alphas + punctuation

aWord = Word(myPrintables)
someText = OneOrMore(aWord)

outputText = someText.parseString(TEXT)

finalOutput = ' '.join(outputText)
print finalOutput