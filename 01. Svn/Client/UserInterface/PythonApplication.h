// Add the following declaration/member block related section:
#if defined(ENABLE_PASSIVE_ATTR)
	#include "PythonPassiveAttr.h"
#endif

// Find this line:
CPythonMountUpGrade m_pyMountUpGrade;

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	CPythonPassiveAttr m_pyPassiveAttr;
#endif
