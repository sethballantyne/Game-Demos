#include "menuitemcontainer.h"

int SortMethod(SelectableControl ^a, SelectableControl ^b)
{
    return a->CompareTo(b);
}

void MenuItemContainer::AddControl(SelectableControl ^control)
{
    if(nullptr == control)
    {
        throw gcnew ArgumentNullException("control");
    }

    this->items->Add(control);

    if(this->items->Count > 1)
    {
        // de-select the currently selected item. Based on the sort, the 0th element will always
        // be the item with the lowest selectedIndex value in the xml file. It's not gauranteed
        // the items in the xml file are in order, so we have to assume we're getting an item
        // that has a lower selectedIndex value than the current selected item.
        this->items[0]->IsSelected = false;

        this->items->Sort(gcnew Comparison<SelectableControl ^>(SortMethod));
    }
   
    this->items[0]->IsSelected = true;
}

void MenuItemContainer::SelectFirstControl()
{
	if(1 == this->items->Count || 0 == this->currentSelectedItem)
	{
		// control is already selected
		return;
	}

	this->items[this->currentSelectedItem]->IsSelected = false;
	this->currentSelectedItem = 0;
	this->items[currentSelectedItem]->IsSelected = true;

	ResourceManager::GetSoundBuffer("menu_nav")->Play();
	
}

void MenuItemContainer::SelectPreviousControl()
{
    // no control to select if there's only one.
    if(1 == this->items->Count)
    {
        return;
    }

    int newSelectedItem = this->currentSelectedItem;

    // using startingPosition to avoid a possible infinite loop.If we've searched the entire
    // list an can't find anything, break out. 
    int startingPosition = this->currentSelectedItem;

    do
    {
        if(0 == newSelectedItem)
        {
            // we're at the beginning of the list, so loop around to the end.
            newSelectedItem = this->items->Count - 1;
        }
        else
        {
            newSelectedItem--;
        }
    } while(false == this->items[newSelectedItem]->Enabled && newSelectedItem != startingPosition);

    this->items[this->currentSelectedItem]->IsSelected = false;
    this->items[newSelectedItem]->IsSelected = true;
    this->currentSelectedItem = newSelectedItem;

    ResourceManager::GetSoundBuffer("menu_nav")->Play();
}

void MenuItemContainer::SelectNextControl()
{
    if(1 == this->items->Count)
    {
        return;
    }

    int newSelectedItem = this->currentSelectedItem;

    // using startingPosition to avoid a possible infinite loop. If we've searched the entire
    // list an can't find anything, break out. 
    int startingPosition = this->currentSelectedItem;
    do
    {
        if(this->items->Count - 1 == newSelectedItem)
        {
            // at the end of the list, move to the beginning.
            newSelectedItem = 0;
        }
        else
        {
            newSelectedItem++;
        }
    } while(false == this->items[newSelectedItem]->Enabled && newSelectedItem != startingPosition);

    this->items[this->currentSelectedItem]->IsSelected = false;
    this->items[newSelectedItem]->IsSelected = true;
    this->currentSelectedItem = newSelectedItem;

    ResourceManager::GetSoundBuffer("menu_nav")->Play();
}