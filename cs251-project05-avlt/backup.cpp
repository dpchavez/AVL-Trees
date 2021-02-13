	if (abs(LH - RH) > 1)
		{
			cout << "test";
			if (LH > RH)
			{
				cout << "test 1";
				if (heightHelper(Curr->Left->Left) > heightRight(Curr->Left)) // This is a Left-Left case
				{
					cout << "left left case" << endl;
					_RightRotate(Parents, Curr);
				}
					
			    else {    // Left-Right case
					cout << "left right case" << endl;
					_LeftRotate(Curr, Curr->Left);
					_RightRotate(Parents, Curr);
				}				
			}
			
			else 
			{
				if (heightHelper(Curr->Right->Left) > heightRight(Curr->Right))  //Right Left case
					{
						cout << "right left case" << endl;
						_RightRotate(Curr, Curr->Right);
						_LeftRotate(Parents, Curr);
					}
				else {
					cout << "right right case" << endl;
					_LeftRotate(Parents, Curr);
				}
			}
		}	