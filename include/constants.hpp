#pragma once

#define LOCALHOST "127.0.0.1"
#define SENDPORT 54321
#define LISTENPORT 54320
#define UDP_BUFFER_SIZE 1024

namespace URack {

const float EPSILON = std::numeric_limits<float>::epsilon();
const float OSC_UPDATE_PERIOD = 0.001f;

} // namespace URack
