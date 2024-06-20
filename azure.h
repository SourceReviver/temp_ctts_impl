#pragma once

// All headers won't includes each other, thus completely independent
// However, the implmentation in `azure.cpp` can refer things defined in all headers.

#include "azure_config_file.h"
#include "azure_config_widget.h"
#include "azure_service.h"