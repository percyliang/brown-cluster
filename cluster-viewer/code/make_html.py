import sys,itertools

style = open(sys.argv[1] + '/style.css').read()

def get_word_rows():
    for line in sys.stdin:
        path, word, count = line.split('\t')
        count = int(count)
        yield path,word,count

def get_cluster_rows():
    for path, rows in itertools.groupby(get_word_rows(), key=lambda x: x[0]):
        wordcounts = [(w,c) for _,w,c in rows]
        wordcounts.sort(key=lambda w_c: -w_c[1])

        yield path, len(wordcounts), wordcounts[:50], wordcounts

def htmlescape(s):
    return s.replace('&','&amp;').replace('<','&lt;').replace('>','&gt;')

def wc_table(wordcounts, tdword=''):
    r = ['<table>']
    for i,(w,c) in enumerate(wordcounts):
        r.append('<tr><td>{} <td class="{}">{} <td class=tdcount>{:,}'.format(i+1, tdword, htmlescape(w), c))
    r.append('</table>')
    return '\n'.join(r)

def top(wc, th):
  cutoff = int(wc[0][1] * th)
  res = []
  for (w,c) in wc:
    if c > cutoff: res.append((w,c))
  return res

for path, nwords, wordcounts, allwc in get_cluster_rows():
    # wc1 = ' '.join("<span class=w>{w}</span>&thinsp;<span class=c>[{c}]</span>".format(
    #     w=htmlescape(w), c=c) for w,c in wordcounts)
    wc1 = ' '.join("<span class=w>{w}</span>".format(
        w=htmlescape(w)) for w,c in top(wordcounts, 0.01))

    print("""
    <tr>
    <td class=path>^<a target=_blank href="paths/{path}.html">{path}</a> <span class=count>({nwords})</span>
    <td class=words>{wc}
    """.format(path=path, nwords=nwords, wc=wc1))
    print("</tr>")

    with open(sys.argv[2] + '/paths/{path}.html'.format(**locals()),'w') as f:
        print("""<style>{style}</style>""".format(**locals()), file=f)
        print("""<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">""", file=f)
        print("<a href=../cluster_viewer.html>back to cluster viewer</a>", file=f)
        print("<h1>cluster path {path}</h1>".format(path=path), file=f)

        print("{n:,} words, {t:,} tokens".format(n=nwords, t=sum(c for w,c in allwc)), file=f)
        print("<a href='#freq'>freq</a> <a href='#alpha'>alpha</a> <a href='#suffix'>suffix</a>", file=f)

        print("<a name=freq><h2>Words in frequency order</h2></a>", file=f)
        allwc.sort(key=lambda w_c: (-w_c[1],w_c[0]))
        print(wc_table(allwc), file=f)
        # wc1 = ' '.join("<span class=w>{w}</span>&nbsp;<span class=c>({c})</span>".format(
        #     w=htmlescape(w), c=c) for w,c in allwc)
        # print>>f, wc1

        print("<a name=alpha><h2>Words in alphabetical order</h2></a>", file=f)
        allwc.sort(key=lambda w_c1: (w_c1[0],-w_c1[1]))
        print(wc_table(allwc), file=f)

        print("<a name=suffix><h2>Words in suffix order</h2></a>", file=f)
        allwc.sort(key=lambda w_c2: (list(reversed(w_c2[0])),-w_c2[1]))
        print(wc_table(allwc, tdword='suffixsort'), file=f)
        # wc1 = ' '.join("<span class=w>{w}</span>&nbsp;<span class=c>({c})</span>".format(
        #     w=htmlescape(w), c=c) for w,c in allwc)
        # print>>f, wc1


