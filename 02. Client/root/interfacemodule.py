# Add to the imports:
if app.ENABLE_PASSIVE_ATTR:
	import passiveAttr


# In `RefreshInventory`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			if self.wndCharacter and self.wndCharacter.wndPassive and self.wndCharacter.wndPassive.IsShow():
				self.wndCharacter.wndPassive.RefreshEquipSlotWindow()


# Add anywhere in the Interface class:
	if app.ENABLE_PASSIVE_ATTR:
		def RegisterPassiveAttrHandler(self):
			if not self.wndCharacter:
				return
			self.wndCharacter.EnsurePassiveWindow()
			passiveAttr.SetHandler(self.wndCharacter.wndPassive)
