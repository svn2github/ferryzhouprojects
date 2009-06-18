#pragma once

namespace fvision {

enum FileType {STILL, AVI, FOLDER};

FileType getFileType(const char* path);

}