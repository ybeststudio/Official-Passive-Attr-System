# In `AddItemData`, extend the if-statement with:
		elif app.ENABLE_PASSIVE_ATTR and item.ITEM_TYPE_PASSIVE == itemType:
			self.__AppendLimitInformation()
			if item.PASSIVE_JOB == itemSubType:
				if app.ENABLE_PASSIVE_ATTR_TOOLTIP:
					self.__AppendPassiveJobAttributeInformation(attrSlot)
					self.__AppendPassiveJobInformation(itemVnum, metinSlot, attrSlot, window_type)
					self.__FinalizePassiveJobToolTipLayout(attrSlot)
				else:
					self.__AppendAttributeInformation(attrSlot)
			else:
				self.__AppendAffectInformation()
			self.AppendWearableInformation()


# In `AddItemData`, extend the if-statement with:
					elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
						if not (app.ENABLE_PASSIVE_ATTR and app.ENABLE_PASSIVE_ATTR_TOOLTIP and item.ITEM_TYPE_PASSIVE == itemType):
							self.AppendTimerBasedOnWearLastTime(metinSlot)
						#dbg.TraceError("1) REAL_TIME flag On ")


# In `AddItemData`, extend the if-statement with:
				elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					if not (app.ENABLE_PASSIVE_ATTR and app.ENABLE_PASSIVE_ATTR_TOOLTIP and item.ITEM_TYPE_PASSIVE == itemType):
						self.AppendTimerBasedOnWearLastTime(metinSlot)
					#dbg.TraceError("1) REAL_TIME flag On ")


# Add anywhere in the ItemToolTip class:
	if app.ENABLE_PASSIVE_ATTR_TOOLTIP:
		def __AppendPassiveJobAttributeInformation(self, attrSlot):
			if not attrSlot:
				return

			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrType = attrSlot[i][0]
				attrValue = attrSlot[i][1]
				if 0 == attrValue:
					continue

				affectString = self.__GetAffectString(attrType, attrValue)
				if affectString:
					affectColor = self.GetChangeTextLineColor(attrValue)
					self.AppendTextLine(affectString, affectColor)

		def __FinalizePassiveJobToolTipLayout(self, attrSlot):
			minWidth = 280

			if attrSlot:
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					attrType = attrSlot[i][0]
					attrValue = attrSlot[i][1]
					if 0 == attrValue:
						continue

					affectString = self.__GetAffectString(attrType, attrValue)
					if affectString:
						minWidth = max(minWidth, app.GetTextWidth(affectString) + 40)

			if self.toolTipWidth < minWidth:
				self.toolTipWidth = minWidth
				self.ResizeToolTip()

			self.AlignTextLineHorizonalCenter()

		def __AppendPassiveJobInformation(self, item_vnum, sockets, attributes, window_type):
			item.SelectItem(item_vnum)

			max_hour = item.GetValue(1)
			if max_hour > 0:
				self.AppendSpace(5)
				self.AppendTextLine(
					localeInfo.PASSIVE_JOB_MAX_TIME % localeInfo.SecondToDHM(max_hour * 60 * 60),
					self.NORMAL_COLOR)

			remain_sec = 0
			if sockets:
				remain_sec = sockets[0]

			if remain_sec > 0:
				self.AppendSpace(5)
				self.AppendTextLine(
					localeInfo.PASSIVE_JOB_LEFT_TIME % localeInfo.SecondToDHM(remain_sec),
					self.NORMAL_COLOR)
