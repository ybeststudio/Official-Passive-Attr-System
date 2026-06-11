// Find this line:
void initMount();

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
void initpassiveAttr();
#endif
