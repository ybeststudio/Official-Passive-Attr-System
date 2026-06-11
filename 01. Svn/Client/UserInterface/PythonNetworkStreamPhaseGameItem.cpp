// In `bool CPythonNetworkStream::RecvSpecialEffect()`, extend the switch statement with:
#if defined(ENABLE_PASSIVE_ATTR)
		case SE_PASSIVE_EFFECT:
			effect = CInstanceBase::EFFECT_PASSIVE;
			break;
#endif
