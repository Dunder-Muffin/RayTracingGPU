#!/bin/bash
PROJECT=${1:-RayTracing}
BUILD_TYPE=${2:-rel}

./Builds/${PROJECT}/${PROJECT}-${BUILD_TYPE}.exe "-project -${PROJECT}"