#!/bin/bash

set -ex

# change this if you fork the repo and want to push you own image
readonly AUTHOR="mdegans"
readonly PROJ_NAME="gstcudaplugin"

# this is so much more arcane than python, but people frown on build scripts written in python
# https://stackoverflow.com/questions/59895/how-to-get-the-source-directory-of-a-bash-script-from-within-the-script-itself
readonly THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TAG_SUFFIX=$(git rev-parse --abbrev-ref HEAD)
if [[ $TAG_SUFFIX == "master" ]]; then
    TAG_SUFFIX="latest"
fi
readonly DOCKERFILE_BASENAME="${TAG_SUFFIX}.Dockerfile"
readonly DOCKERFILE="$THIS_DIR/$DOCKERFILE_BASENAME"
readonly VERSION=$(head -n 1 $THIS_DIR/VERSION)
readonly TAG_BASE="$AUTHOR/$PROJ_NAME"
readonly TAG_FULL="$TAG_BASE:$VERSION"

echo "Building $TAG_FULL from $DOCKERFILE"

docker build --rm -f $DOCKERFILE \
    --build-arg DSFILTER_TAG=${TAG_SUFFIX} \
    --build-arg AUTHOR=${AUTHOR} \
    -t $TAG_FULL \
    $THIS_DIR $@
docker tag "$TAG_FULL" "$TAG_BASE:$TAG_SUFFIX"
