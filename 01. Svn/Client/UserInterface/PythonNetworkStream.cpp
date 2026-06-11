// Find this line:
Set(HEADER_GC_MOUNT_UP_GRADE_CHAT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMountUpGradeChat), STATIC_SIZE_PACKET));

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		Set(HEADER_GC_PASSIVE_ATTR, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPassiveAttr), STATIC_SIZE_PACKET));
#endif
