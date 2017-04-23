#ifndef INTERPRET_H
#define INTERPRET_H

#include <string>
#include <reindeergl/OpenGL.h>

bool InterpGravityFile(const std::string& fileName, std::shared_ptr<GravityPlane>& grvty);
#endif
