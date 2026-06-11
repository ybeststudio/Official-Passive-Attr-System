// Find this line:
t.wPos = item->GetCell();

// Add after it:
#if defined(__PASSIVE_ATTR__)
	EncodePlayerItemWirePos(t.bWindow, t.wPos);
#endif
