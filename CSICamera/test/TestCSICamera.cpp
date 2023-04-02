#include <iostream>

#include "CSICamera.h"

int main() {

    CSICamera *camera = new CSICamera();

    camera->save_frame(".");

}