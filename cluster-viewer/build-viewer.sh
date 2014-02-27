#!/bin/bash
set -e

CODEDIR=`dirname $0`/code

if [ "$#" -lt "1" ] || [ "$#" -gt "2" ]
then
  echo "Usage: $0 path/to/clusters.out/paths [outdir]" 1>&2
  echo 1>&2
  echo "Builds an HTML cluster viewer." 1>&2
  echo 1>&2
  exit
fi
MAPFILE=$1
CATCMD=cat
if [[ "$MAPFILE" == *.gz ]]
then
  CATCMD='gunzip -c'
fi
OUTDIR=clusters
if [ $# -eq 2 ]
then
  OUTDIR=$2
fi

echo "Creating output in $OUTDIR ..." 1>&2
mkdir -p $OUTDIR
mkdir -p $OUTDIR/paths
$CATCMD $MAPFILE | python $CODEDIR/make_html.py $CODEDIR $OUTDIR > $OUTDIR/htmlrows.html
python $CODEDIR/final.py $CODEDIR $OUTDIR > $OUTDIR/cluster_viewer.html
echo "Done. View clusters in $OUTDIR/cluster_viewer.html" 1>&2

