# Add anywhere in the AffectImage class:
	if app.ENABLE_PASSIVE_ATTR:
		def __ResolveDescriptionText(self, description, value=0):
			if callable(description):
				return description(value)
			return description

		def UpdatePassiveJobDeckDescription(self):
			if not self.description:
				return

			toolTip = self.__ResolveDescriptionText(self.description, 0)
			itemToolTip = uiToolTip.ItemToolTip()
			itemToolTip.Hide()

			window_type = player.WEAR_PASSIVE_ATTR
			cell = player.WEAR_PASSIVE_ATTR_UP
			hasBonus = False

			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				(attrType, attrValue) = player.GetItemAttribute(window_type, cell, i)
				if attrValue != 0:
					affectString = itemToolTip.GetAffectString(attrType, attrValue)
					if affectString:
						if not hasBonus:
							toolTip += "\\n"
							hasBonus = True
						toolTip += affectString
						toolTip += "\\n"

			remain_sec = player.GetItemMetinSocket(window_type, cell, 0)
			if remain_sec > 0:
				if not hasBonus:
					toolTip += "\\n"
				toolTip += "%s : %s" % (localeInfo.LEFT_TIME, localeInfo.SecondToDHM(remain_sec))

			self.SetToolTipText(toolTip, AFFECT_TOOLTIP_POS_X, AFFECT_TOOLTIP_POS_Y)


# In `__IsOfficialAffectEnabled`, extend the if-statement with:
		if _affectType == chr.AFFECT_PASSIVE_JOB_DECK:
			return app.ENABLE_PASSIVE_ATTR

# In `BINARY_NEW_AddAffect`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR and affect == chr.AFFECT_PASSIVE_JOB_DECK:
			import passiveAttr
			if value == passiveAttr.PASSIVE_ATTR_PAGE_UP:
				filename = "d:/ymir work/ui/skill/common/affect/passive_attr_up.sub"
			else:
				filename = "d:/ymir work/ui/skill/common/affect/passive_attr_down.sub"

			image = AffectImage()
			image.SetParent(self)
			image.LoadImage(filename)
			image.SetScale(0.7, 0.7)
			passiveDesc = localeInfo.TOOLTIP_PASSIVE_JOB_DECK
			if callable(passiveDesc):
				passiveDesc = passiveDesc(0)
			image.SetDescription(passiveDesc)
			image.SetDuration(duration)
			image.SetAffect(affect)
			image.SetClock(False)
			image.UpdatePassiveJobDeckDescription()
			image.SetSkillAffectFlag(FALSE)
			image.Show()
			self.affectImageDict[affect] = image
			self.__ArrangeImageList()
			return


# In `BINARY_NEW_UpdateAffect`, extend the if-statement with:
			if app.ENABLE_PASSIVE_ATTR and affect == chr.AFFECT_PASSIVE_JOB_DECK:
				import passiveAttr
				if value == passiveAttr.PASSIVE_ATTR_PAGE_UP:
					filename = "d:/ymir work/ui/skill/common/affect/passive_attr_up.sub"
				else:
					filename = "d:/ymir work/ui/skill/common/affect/passive_attr_down.sub"
				image.LoadImage(filename)
				image.SetScale(0.7, 0.7)
				image.UpdatePassiveJobDeckDescription()
