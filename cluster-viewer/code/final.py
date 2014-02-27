import sys
template = open(sys.argv[1] + '/template.html').read()
final = template
final = final.replace('STYLE', open(sys.argv[1] + '/style.css').read())
htmlrows = open(sys.argv[2] + '/htmlrows.html').read()
final = final.replace('TABLE', htmlrows)
print final

