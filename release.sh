#!/bin/sh
version=$1
tmp=$(mktemp)
jq --arg version "$version" '.versions |= [$version] + .' installer/versions.json >  "$tmp" && mv "$tmp" installer/versions.json
