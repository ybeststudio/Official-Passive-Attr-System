import ui
import app
import net
import snd
import chat
import player
import mouseModule
import wndMgr
import item
import uiToolTip
import uiCommon
import localeInfo
import passiveAttr

# passiveattr.py: SLOT_INDEX_JOB — merkez Soul Relic görseli
PASSIVE_UI_SLOT_CENTER = 0

class PassiveWindow(ui.ScriptWindow):

	def __init__(self, wndInventory):
		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.wndInventory = wndInventory
		self.wndPassive = None
		self.wndPassiveJobUpSlot = None
		self.wndPassiveJobDownSlot = None
		self.chargeButton = None
		self.addButton = None
		self.activateButton = None
		self.deckButton1 = None
		self.deckButton2 = None
		self.selectedDeckIndex = 0
		self.toolTipItem = uiToolTip.ItemToolTip()
		self.toolTipItem.SetFollow(TRUE)
		self.toolTipItem.HideToolTip()
		self.questionDialog = None
		self.deckHasRelicByDeck = {0 : FALSE, 1 : FALSE}
		self.materialSlotDataByDeck = {
			0 : self.__CreateDefaultMaterialSlotData(),
			1 : self.__CreateDefaultMaterialSlotData(),
		}
		self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
		self.isRequestingChangePage = FALSE
		self.isRequestingAdd = FALSE
		self.isRequestingCharge = FALSE
		self.isRequestingActivate = FALSE
		self.prevDeckIndexOnChange = -1
		self.thinToolTip = None

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()
		self.RefreshEquipSlotWindow()
		self.SetCenterPosition()
		self.SetTop()
		net.SendPassiveAttrOpen()
		ui.ScriptWindow.Show(self)

	def Close(self, isPacketSend = TRUE):
		if isPacketSend:
			net.SendPassiveAttrClose()
		if self.toolTipItem:
			self.toolTipItem.HideToolTip()
		self.__CloseQuestionDialog()
		self.Hide()

	def Destroy(self):
		self.Close(FALSE)
		self.ClearDictionary()
		self.__Initialize()

	def __Initialize(self):
		self.isLoaded = 0
		self.wndInventory = None
		self.wndPassive = None
		self.wndPassiveJobUpSlot = None
		self.wndPassiveJobDownSlot = None
		self.chargeButton = None
		self.addButton = None
		self.activateButton = None
		self.deckButton1 = None
		self.deckButton2 = None
		self.selectedDeckIndex = 0
		self.toolTipItem = None
		self.questionDialog = None
		self.materialSlotDataByDeck = None
		self.materialSlotData = None
		self.isRequestingChangePage = FALSE
		self.isRequestingAdd = FALSE
		self.isRequestingCharge = FALSE
		self.isRequestingActivate = FALSE
		self.prevDeckIndexOnChange = -1
		self.deckHasRelicByDeck = None
		self.thinToolTip = None

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/passiveattr.py")
		except:
			import exception
			exception.Abort("PassiveWindow.LoadWindow.LoadObject")

		try:
			wndPassive = self.GetChild("passive_attr_sub_slot")
			wndPassiveJobUpSlot = self.GetChild("passive_job_up_slot")
			wndPassiveJobDownSlot = self.GetChild("passive_job_down_slot")
			chargeButton = self.GetChild("ChargeButton")
			addButton = self.GetChild("AddButton")
			activateButton = self.GetChild("ActivateButton")
			deckButton1 = self.GetChild("deck_button1")
			deckButton2 = self.GetChild("deck_button2")
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))

		except:
			import exception
			exception.Abort("PassiveWindow.LoadWindow.BindObject")

		self.wndPassive = wndPassive
		self.wndPassiveJobUpSlot = wndPassiveJobUpSlot
		self.wndPassiveJobDownSlot = wndPassiveJobDownSlot
		self.chargeButton = chargeButton
		self.addButton = addButton
		self.activateButton = activateButton
		self.deckButton1 = deckButton1
		self.deckButton2 = deckButton2
		self.wndPassive.SetSelectEmptySlotEvent(ui.__mem_func__(self.RingSelectEmptySlot))
		self.wndPassive.SetSelectItemSlotEvent(ui.__mem_func__(self.RingSelectItemSlot))
		self.wndPassive.SetUnselectItemSlotEvent(ui.__mem_func__(self.RingUseItemSlot))
		self.wndPassive.SetUseSlotEvent(ui.__mem_func__(self.RingUseItemSlot))
		self.wndPassive.SetOverInItemEvent(ui.__mem_func__(self.RingOverInItem))
		self.wndPassive.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.wndPassive.Show()

		self.__BindPassiveJobSlotEvents(self.wndPassiveJobUpSlot)
		self.__BindPassiveJobSlotEvents(self.wndPassiveJobDownSlot)

		if self.wndPassiveJobUpSlot:
			self.wndPassiveJobUpSlot.Show()
		if self.wndPassiveJobDownSlot:
			self.wndPassiveJobDownSlot.Hide()

		self.deckButton1.SetEvent(ui.__mem_func__(self.__SelectDeck), 0)
		self.deckButton2.SetEvent(ui.__mem_func__(self.__SelectDeck), 1)
		self.chargeButton.SetEvent(ui.__mem_func__(self.__OpenChargeQuestionDialog))
		self.addButton.SetEvent(ui.__mem_func__(self.__AddRelicBonus))
		self.activateButton.SetToggleDownEvent(ui.__mem_func__(self.__OnActivateButtonDown))
		self.activateButton.SetToggleUpEvent(ui.__mem_func__(self.__OnActivateButtonUp))
		self.__RefreshDeckButtonState()
		self.__RefreshButtonState()

		if app.ENABLE_PASSIVE_ATTR_TOOLTIP:
			self.thinToolTip = uiToolTip.ToolTip()
			self.thinToolTip.HideToolTip()
			try:
				helpButton = self.GetChild("HelpButton")
				helpButton.SetOverEvent(ui.__mem_func__(self.__OverInHelpButton))
				helpButton.SetOverOutEvent(ui.__mem_func__(self.__OverOutHelpButton))
			except:
				pass

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def __BindPassiveJobSlotEvents(self, slotWnd):
		if not slotWnd:
			return

		slotWnd.SetSelectEmptySlotEvent(ui.__mem_func__(self.CenterSelectEmptySlot))
		slotWnd.SetSelectItemSlotEvent(ui.__mem_func__(self.CenterSelectItemSlot))
		slotWnd.SetUnselectItemSlotEvent(ui.__mem_func__(self.CenterUseItemSlot))
		slotWnd.SetUseSlotEvent(ui.__mem_func__(self.CenterUseItemSlot))
		slotWnd.SetOverInItemEvent(ui.__mem_func__(self.CenterOverInItem))
		slotWnd.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

	def __GetPassiveWearWindow(self):
		return player.WEAR_PASSIVE_ATTR

	def __GetPassiveWearCell(self):
		return player.WEAR_PASSIVE_ATTR_UP

	def __GetPassiveWearItemIndex(self):
		if not self.deckHasRelicByDeck[self.selectedDeckIndex]:
			return 0
		itemVNum = player.GetItemIndex(self.__GetPassiveWearWindow(), self.__GetPassiveWearCell())
		if itemVNum == 0:
			return 0
		return itemVNum

	def __GetPassiveWearItemCount(self):
		if self.__GetPassiveWearItemIndex() == 0:
			return 0
		itemCount = player.GetItemCount(self.__GetPassiveWearWindow(), self.__GetPassiveWearCell())

	def __GetMaterialSlotOrder(self):
		return (1, 2, 3, 4)

	def __GetCurrentPage(self):
		if self.selectedDeckIndex == 0:
			return passiveAttr.PASSIVE_ATTR_PAGE_UP
		return passiveAttr.PASSIVE_ATTR_PAGE_DOWN

	def __GetMaterialPacketPositions(self):
		positions = []
		for slotIndex in self.__GetMaterialSlotOrder():
			slotPos = self.materialSlotData[slotIndex]["pos"]
			if slotPos < 0:
				return None
			positions.append(slotPos)
		return positions

	def __ShowResultMessage(self, subHeader, result):
		if result == passiveAttr.PASSIVE_ATTR_GC_OK:
			if subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_ADD:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_SUCCESS)
			elif subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_CHARGE:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_CHARGE_SUCCESS_MSG)
			return

		if result == passiveAttr.PASSIVE_ATTR_GC_FAIL_MAX_ATTR:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL_MAX_ATTR)
		elif result == passiveAttr.PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_JOB_ITEM:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL_NOT_ENOUGH_JOB_ITEM)
		elif result == passiveAttr.PASSIVE_ATTR_GC_FAIL_FULL_TIME:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_CHARGE_FAIL_FULL_TIME)
		elif result == passiveAttr.PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_NOT_ENOUGH_SUB_ITEM)
		elif result == passiveAttr.PASSIVE_ATTR_GC_FAIL_COOLTIME:
			if subHeader not in (
				passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_CHANGE_WINDOW,
				passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_OPEN,
			):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_COOLTIME_FAIL_MSG)
		elif result == passiveAttr.PASSIVE_ATTR_GC_FAIL_CONTROL:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_CONTROL_FAIL_MSG)
		elif subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_ADD:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL)

	def PassiveAttrProcess(self, subHeader, page, result, active, remainSec, hasRelic = 0):
		self.isRequestingChangePage = FALSE
		self.isRequestingAdd = FALSE
		self.isRequestingCharge = FALSE
		self.isRequestingActivate = FALSE

		deckIdx = 0
		if page == passiveAttr.PASSIVE_ATTR_PAGE_DOWN:
			deckIdx = 1

		if subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_OPEN:
			if result == passiveAttr.PASSIVE_ATTR_GC_OK:
				self.selectedDeckIndex = deckIdx
				self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
				self.deckHasRelicByDeck[deckIdx] = (hasRelic != 0)
			self.__ShowResultMessage(subHeader, result)
		elif subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_CHANGE_WINDOW:
			if result == passiveAttr.PASSIVE_ATTR_GC_OK:
				self.selectedDeckIndex = deckIdx
				self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
				self.deckHasRelicByDeck[deckIdx] = (hasRelic != 0)
			elif self.prevDeckIndexOnChange >= 0:
				self.selectedDeckIndex = self.prevDeckIndexOnChange
				self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
			self.prevDeckIndexOnChange = -1
			self.__ShowResultMessage(subHeader, result)
		elif subHeader == passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_USE_ITEM_JOB:
			if result == passiveAttr.PASSIVE_ATTR_GC_OK:
				self.deckHasRelicByDeck[deckIdx] = (hasRelic != 0)
			self.__ShowResultMessage(subHeader, result)
		else:
			self.__ShowResultMessage(subHeader, result)

		if subHeader in (
			passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_OPEN,
			passiveAttr.PASSIVE_ATTR_SUBHEADER_GC_ACTIVATE_DEACTIVATE,
		):
			if self.activateButton:
				if active:
					self.activateButton.Down()
				else:
					self.activateButton.SetUp()

		self.__RefreshDeckButtonState()
		self.__RefreshButtonState()
		self.RefreshEquipSlotWindow()

		if self.__HasPassiveRelic() and remainSec > 0:
			player.SetItemMetinSocket(self.__GetPassiveWearWindow(), self.__GetPassiveWearCell(), 0, remainSec)

	def __CreateDefaultMaterialSlotData(self):
		return {
			1 : {"vnum" : 30255, "pos" : -1},
			2 : {"vnum" : 30258, "pos" : -1},
			3 : {"vnum" : 30256, "pos" : -1},
			4 : {"vnum" : 30257, "pos" : -1},
		}

	def __RefreshDeckButtonState(self):
		if not self.deckButton1 or not self.deckButton2:
			return

		if self.selectedDeckIndex == 0:
			self.deckButton1.Down()
			self.deckButton2.SetUp()
		else:
			self.deckButton1.SetUp()
			self.deckButton2.Down()

	def __SelectDeck(self, deckIndex):
		if deckIndex == self.selectedDeckIndex:
			return

		self.prevDeckIndexOnChange = self.selectedDeckIndex
		self.selectedDeckIndex = deckIndex
		self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
		self.__RefreshDeckButtonState()
		self.__RefreshMaterialSlots()
		self.RefreshEquipSlotWindow()
		self.__RefreshButtonState()

		page = passiveAttr.PASSIVE_ATTR_PAGE_UP if deckIndex == 0 else passiveAttr.PASSIVE_ATTR_PAGE_DOWN
		net.SendPassiveAttrChangePage(page)

	def __ClearMaterialPosFromOtherDecks(self, invenPos, exceptDeckIndex):
		if invenPos < 0 or not self.materialSlotDataByDeck:
			return

		for deckIndex, deckData in self.materialSlotDataByDeck.items():
			if deckIndex == exceptDeckIndex:
				continue

			for slotIndex in self.__GetMaterialSlotOrder():
				if deckData[slotIndex]["pos"] == invenPos:
					deckData[slotIndex]["pos"] = -1

	def __AssignMaterialSlotPos(self, slotIndex, invenPos):
		self.__ClearMaterialPosFromOtherDecks(invenPos, self.selectedDeckIndex)
		self.materialSlotData[slotIndex]["pos"] = invenPos

	def __ValidateMaterialSlots(self):
		for slotIndex in self.__GetMaterialSlotOrder():
			itemData = self.materialSlotData[slotIndex]
			slotPos = itemData["pos"]
			if slotPos < 0:
				continue

			if player.GetItemIndex(slotPos) != itemData["vnum"]:
				itemData["pos"] = -1

	def __RefreshMaterialSlots(self):
		self.__ValidateMaterialSlots()

		for slotIndex in self.__GetMaterialSlotOrder():
			itemData = self.materialSlotData[slotIndex]
			slotPos = itemData["pos"]
			if slotPos >= 0:
				itemVNum = player.GetItemIndex(slotPos)
				itemCount = player.GetItemCount(slotPos)
				if itemCount <= 1:
					itemCount = 0
				self.wndPassive.SetItemSlot(slotIndex, itemVNum, itemCount)
			else:
				self.wndPassive.ClearSlot(slotIndex)

		self.wndPassive.RefreshSlot()

	def __HasPassiveRelic(self):
		return self.__GetPassiveWearItemIndex() != 0

	def __IsPassiveRelicActive(self):
		if not self.__HasPassiveRelic():
			return FALSE

		return player.GetItemMetinSocket(self.__GetPassiveWearWindow(), player.WEAR_PASSIVE_ATTR_UP, 1) != 0

	def __RefreshButtonState(self):
		hasPassiveRelic = self.__HasPassiveRelic()
		prevAvail = getattr(self, "__passiveRelicBtnAvail", None)
		if prevAvail != hasPassiveRelic:
			self.__passiveRelicBtnAvail = hasPassiveRelic
			# Charge/Add should remain clickable; guard inside handlers.
			if hasPassiveRelic:
				self.activateButton.Enable()
			else:
				self.activateButton.Disable()

		if not hasPassiveRelic:
			return

		if self.__IsPassiveRelicActive():
			self.activateButton.Down()
		else:
			self.activateButton.SetUp()

	def __CloseQuestionDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None

	def __OpenChargeQuestionDialog(self):
		if not self.__HasPassiveRelic():
			if self.chargeButton:
				self.chargeButton.SetUp()
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL_NOT_ENOUGH_JOB_ITEM)
			return

		self.__CloseQuestionDialog()
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.PASSIVE_ATTR_CHARGE_QUESTION)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.__AcceptCharge))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.__CloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def __AcceptCharge(self):
		self.__CloseQuestionDialog()
		positions = self.__GetMaterialPacketPositions()
		if not positions:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_NOT_ENOUGH_SUB_ITEM)
			return
		self.isRequestingCharge = TRUE
		net.SendPassiveAttrCharge(self.__GetCurrentPage(), positions[0], positions[1], positions[2], positions[3])

	def __AddRelicBonus(self):
		if not self.__HasPassiveRelic():
			if self.addButton:
				self.addButton.SetUp()
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_ADD_FAIL_NOT_ENOUGH_JOB_ITEM)
			return

		positions = self.__GetMaterialPacketPositions()
		if not positions:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PASSIVE_ATTR_NOT_ENOUGH_SUB_ITEM)
			return

		self.isRequestingAdd = TRUE
		net.SendPassiveAttrAdd(self.__GetCurrentPage(), positions[0], positions[1], positions[2], positions[3])

	def __OnActivateButtonDown(self):
		if not self.__HasPassiveRelic():
			self.__RefreshButtonState()
			return

		if self.__IsPassiveRelicActive():
			return

		if self.activateButton:
			self.activateButton.SetUp()

		self.__OpenActivateQuestionDialog()

	def __OnActivateButtonUp(self):
		if not self.__HasPassiveRelic():
			self.__RefreshButtonState()
			return

		if not self.__IsPassiveRelicActive():
			self.__RefreshButtonState()
			return

		self.__SendActivateDeactivate()

	def __OpenActivateQuestionDialog(self):
		self.__CloseQuestionDialog()
		questionDialog = uiCommon.QuestionDialog2()
		questionDialog.SetText1(localeInfo.PASSIVE_JOB_FIRST_ACTIVATE_POPUP1)
		questionDialog.SetText2(localeInfo.PASSIVE_JOB_FIRST_ACTIVATE_POPUP2)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.__AcceptActivate))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.__CancelActivateQuestion))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def __CancelActivateQuestion(self):
		self.__CloseQuestionDialog()
		self.__RefreshButtonState()

	def __AcceptActivate(self):
		self.__CloseQuestionDialog()
		self.__SendActivateDeactivate()

	def __SendActivateDeactivate(self):
		if self.isRequestingActivate:
			self.__RefreshButtonState()
			return

		self.isRequestingActivate = TRUE
		net.SendPassiveAttrActivateDeactivate(self.__GetCurrentPage())

	def __ShowToolTip(self):
		if not self.toolTipItem:
			return

		self.toolTipItem.SetFollow(TRUE)
		self.toolTipItem.SetToolTipPosition(-1, -1)
		self.toolTipItem.SetInventoryItem(self.__GetPassiveWearCell(), self.__GetPassiveWearWindow())
		self.toolTipItem.ShowToolTip()

	def RingOverInItem(self, slotIndex):
		if mouseModule.mouseController.isAttached():
			return

		if slotIndex in self.materialSlotData:
			itemData = self.materialSlotData[slotIndex]
			slotPos = itemData["pos"]
			self.toolTipItem.SetFollow(TRUE)
			self.toolTipItem.SetToolTipPosition(-1, -1)
			if slotPos >= 0 and player.GetItemIndex(slotPos) == itemData["vnum"]:
				self.toolTipItem.SetInventoryItem(slotPos)
			else:
				self.toolTipItem.SetItemToolTip(itemData["vnum"])
			self.toolTipItem.ShowToolTip()

	def CenterOverInItem(self, slotIndex):
		if mouseModule.mouseController.isAttached():
			return

		if slotIndex != PASSIVE_UI_SLOT_CENTER:
			return

		if self.__GetPassiveWearItemIndex() == 0:
			return

		self.__ShowToolTip()

	def OverOutItem(self):
		if self.toolTipItem:
			self.toolTipItem.HideToolTip()

	if app.ENABLE_PASSIVE_ATTR_TOOLTIP:
		def __OverInHelpButton(self):
			if not self.thinToolTip:
				return

			self.thinToolTip.ClearToolTip()
			self.thinToolTip.AutoAppendTextLine(localeInfo.PASSIVE_ATTR_TOOLTIP_TITLE, self.thinToolTip.TITLE_COLOR)
			for line in (
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE1,
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE2,
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE3,
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE4,
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE5,
				localeInfo.PASSIVE_ATTR_TOOLTIP_LINE6,
			):
				self.thinToolTip.AutoAppendTextLine(line)
			self.thinToolTip.AlignHorizonalCenter()
			self.thinToolTip.ShowToolTip()

		def __OverOutHelpButton(self):
			if self.thinToolTip:
				self.thinToolTip.HideToolTip()

	def __TryPassiveRelicExtractFromAttached(self):
		if not mouseModule.mouseController.isAttached():
			return FALSE

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

		if attachedSlotType != player.SLOT_TYPE_INVENTORY:
			return FALSE

		if attachedItemVNum not in (100100, 100101):
			return FALSE

		if self.__GetPassiveWearItemIndex() == 0:
			return FALSE

		mouseModule.mouseController.DeattachObject()
		net.SendPassiveAttrUseItemJob(self.__GetCurrentPage(), attachedSlotPos)
		return TRUE

	def RingSelectItemSlot(self, slotIndex):
		if self.__TryPassiveRelicExtractFromAttached():
			return

		if slotIndex in self.materialSlotData:
			self.SelectMaterialItemSlot(slotIndex)

	def RingSelectEmptySlot(self, selectedSlotPos):
		if self.__TryPassiveRelicExtractFromAttached():
			return

		if selectedSlotPos in self.materialSlotData:
			self.SelectEmptyMaterialSlot(selectedSlotPos)

	def RingUseItemSlot(self, slotIndex):
		if slotIndex in self.materialSlotData:
			self.SelectMaterialItemSlot(slotIndex)

	def CenterSelectItemSlot(self, slotIndex):
		if self.__TryPassiveRelicExtractFromAttached():
			return

		self.CenterUseItemSlot(slotIndex)

	def CenterSelectEmptySlot(self, selectedSlotPos):
		if selectedSlotPos != PASSIVE_UI_SLOT_CENTER:
			return

		if not mouseModule.mouseController.isAttached():
			return

		if self.__TryPassiveRelicExtractFromAttached():
			return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()
		attachedWindow = player.SlotTypeToInvenType(attachedSlotType)

		if player.RESERVED_WINDOW == attachedWindow:
			return

		mouseModule.mouseController.DeattachObject()

		if attachedWindow == player.WEAR_PASSIVE_ATTR:
			return

		net.SendItemUsePacket(attachedSlotPos)
		self.RefreshEquipSlotWindow()

	def SelectEmptyMaterialSlot(self, selectedSlotPos):
		if not selectedSlotPos in self.materialSlotData:
			return

		if not mouseModule.mouseController.isAttached():
			return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

		if player.SLOT_TYPE_INVENTORY != attachedSlotType:
			return

		if attachedItemVNum != self.materialSlotData[selectedSlotPos]["vnum"]:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Bu item bu yuvaya yerlestirilemez.")
			return

		self.__AssignMaterialSlotPos(selectedSlotPos, attachedSlotPos)
		mouseModule.mouseController.DeattachObject()
		self.__RefreshMaterialSlots()

	def SelectMaterialItemSlot(self, selectedSlotPos):
		if not selectedSlotPos in self.materialSlotData:
			return

		self.materialSlotData[selectedSlotPos]["pos"] = -1
		self.__RefreshMaterialSlots()

	def TryAttachMaterialFromInventory(self, inventorySlot):
		if inventorySlot < 0:
			return FALSE

		itemVNum = player.GetItemIndex(inventorySlot)
		if itemVNum == 0:
			return FALSE

		for slotIndex in self.__GetMaterialSlotOrder():
			itemData = self.materialSlotData[slotIndex]
			if itemData["vnum"] != itemVNum:
				continue

			currentPos = itemData["pos"]
			if currentPos == inventorySlot:
				return TRUE

			if currentPos >= 0 and player.GetItemIndex(currentPos) == itemData["vnum"]:
				continue

			self.__AssignMaterialSlotPos(slotIndex, inventorySlot)
			self.__RefreshMaterialSlots()
			return TRUE

		return FALSE

	def CenterUseItemSlot(self, slotIndex):
		if slotIndex != PASSIVE_UI_SLOT_CENTER:
			return

		if self.__GetPassiveWearItemIndex() == 0:
			return

		snd.PlaySound("sound/ui/click.wav")
		net.SendPassiveAttrUseItemJob(self.__GetCurrentPage(), 0xFFFF)

	def RefreshEquipSlotWindow(self):
		itemVNum = self.__GetPassiveWearItemIndex()
		itemCount = self.__GetPassiveWearItemCount()
		if itemCount <= 1:
			itemCount = 0

		if self.selectedDeckIndex == 0:
			activeJobSlot = self.wndPassiveJobUpSlot
			inactiveJobSlot = self.wndPassiveJobDownSlot
		else:
			activeJobSlot = self.wndPassiveJobDownSlot
			inactiveJobSlot = self.wndPassiveJobUpSlot

		if activeJobSlot:
			activeJobSlot.Show()
			activeJobSlot.SetItemSlot(PASSIVE_UI_SLOT_CENTER, itemVNum, itemCount)
			activeJobSlot.RefreshSlot()

		if inactiveJobSlot:
			inactiveJobSlot.Hide()

		if activeJobSlot:
			activeJobSlot.SetTop()

		self.__RefreshMaterialSlots()
		self.__RefreshDeckButtonState()
		self.__RefreshButtonState()

	def OnUpdate(self):
		if self.IsShow():
			if self.toolTipItem and self.toolTipItem.IsShow() and self.toolTipItem.followFlag:
				uiToolTip.ToolTip.OnUpdate(self.toolTipItem)
			if app.ENABLE_PASSIVE_ATTR_TOOLTIP and self.thinToolTip and self.thinToolTip.IsShow():
				self.thinToolTip.OnUpdate()

PassiveAttr = PassiveWindow
