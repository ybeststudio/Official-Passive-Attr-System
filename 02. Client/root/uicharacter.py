# Add to the imports:
if app.ENABLE_PASSIVE_ATTR:
	import uipassiveattr


# In `__init__`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			self.wndPassive = None


# In `__BindObject`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			self.passiveexpandedbtn = self.GetChild("passive_expanded_btn")
			self.passiveexpandedbtn.SetEvent(ui.__mem_func__(self.__ShowPassiveButton))


# In `Destroy`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			if self.wndPassive:
				self.wndPassive.Destroy()
				self.wndPassive = 0

# Add anywhere in the CharacterWindow class:
	if app.ENABLE_PASSIVE_ATTR:
		def EnsurePassiveWindow(self):
			if not self.wndPassive:
				self.wndPassive = uipassiveattr.PassiveWindow(self)

		def __ShowPassiveButton(self):
			self.EnsurePassiveWindow()
			if self.wndPassive.IsShow():
				self.wndPassive.Close()
			else:
				self.wndPassive.Show()

		def TryAttachPassiveMaterialFromInventory(self, inventorySlot):
			if not self.wndPassive or not self.wndPassive.IsShow():
				return FALSE

			return self.wndPassive.TryAttachMaterialFromInventory(inventorySlot)


# In `RefreshCharacter`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			if self.wndPassive and self.wndPassive.IsShow():
				self.wndPassive.RefreshEquipSlotWindow()
