#include "main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class imp_res : public Restaurant
{
	///////////////////////////////////////////////////////////		MEMBER VARIABLE, FUNCTION	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
public:
	Restaurant::customer *tableCus; 		// Danh sách liên kết đôi vòng mô tả bàn tròn (đang được phục vụ)
	int totalCusTable;

	Restaurant::customer *queueCus; 		// Queue mô tả hàng chờ (chưa được phục vụ và sẽ được phục vụ)
	int totalCusQueue;

	Restaurant::customer *anotherCus; 		// Queue dùng để lưu thứ tự các vị khách đến sớm nhất -> muộn nhất.
	int totalCusAnother;

	Restaurant::customer *subList;			// Danh sách liên kết đôi vòng lưu trữ dãy (chuỗi) con trong hàm unlimited_void
	int totalCusSubList;


	///////////////////////////////////////////////////////////		CONSTRUCTOR && DESTRUCTOR	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void deleteCus 	(Restaurant::customer *&head, int &size, const int option)
	{
		Restaurant::customer *tmpHead = head;
		if (size > 0) {
			for (int i = 0; i < size; i++) {
				tmpHead -> prev = nullptr;
				tmpHead = tmpHead->next;
			}

			if (option == 1) {
				tmpHead = head;
				head = head->next;
				tmpHead->next = nullptr;
			}			

			while (head != nullptr) {
				Restaurant::customer *deletedNode = head;
				head = head->next;
				delete deletedNode; COUNTDELETE--;
				deletedNode = nullptr;
				size--;
			}
		}
		tmpHead = nullptr;
	}
	/* Constructor */
	imp_res			()
	{
		tableCus = nullptr;
		totalCusTable = 0;

		queueCus = nullptr;
		totalCusQueue = 0;

		anotherCus = nullptr;
		totalCusAnother = 0;

		subList = nullptr;
		totalCusSubList = 0;
	}
	/* Destructor */
	~imp_res		()
	{
		deleteCus(tableCus, totalCusTable, 1);
		deleteCus(queueCus, totalCusQueue, 2);
		deleteCus(anotherCus, totalCusAnother, 2);
		deleteCus(subList, totalCusSubList, 1);
	}






	///////////////////////////////////////////////////////////		AUXILIRAY CLASS	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	bool checkExist			(Restaurant::customer *&head, int &size, string name)
	{
		Restaurant::customer *tmpCus = head;
		for (int i = 0; i < size; i++) {
			if (tmpCus->name == name) {
				return true;
			}
			tmpCus = tmpCus->next;
		}
		tmpCus = nullptr;
		return false;
	}
	
	bool checkExistTable 	(string name)
	{
		return checkExist(tableCus, totalCusTable, name);
	}

	bool checkExistQueue 	(string name)
	{
		return checkExist(queueCus, totalCusQueue, name);
	}

	bool is_identicalName 	(const string name)
	{
		return (checkExistTable(name) || checkExistQueue(name));
	}

	void sumEnergy			(int &sumPositive, int &sum)
	{
		sumPositive = 0;
		sum = 0;
		
		Restaurant::customer *tmpCus = anotherCus;
		for (int i = 0; i < totalCusAnother; i++) {
			if (tmpCus->energy > 0) {
				sumPositive += (tmpCus->energy);
			}
			else if (tmpCus->energy < 0) {
				sum += (tmpCus->energy);
			}
			tmpCus = tmpCus->next;
		}
		tmpCus = nullptr;
	}

	int kickCus 			(int option)
	// option = 1: đuổi tất cả chú thuật sư
	// option = 2: đuổi tất cả oán linh
	// Trả về cặp số lượng khách bị kick trong bàn tròn
	{
		int count;
		
		Restaurant::customer *tmpCus = anotherCus;	
		Restaurant::customer *tmpCusNext;
		while (tmpCus != nullptr) {
			tmpCusNext = tmpCus->next;
			if (((option == 1 && tmpCus->energy > 0) || (option == 2 && tmpCus->energy < 0))) {
				if (checkExistTable(tmpCus->name) && !checkExistQueue(tmpCus->name)) {
					pop_TableCus(tmpCus->name);
					pop_AnotherCus(tmpCus->name);
					count++;
				}
				else {
					pop_QueueCus(tmpCus->name);
					pop_AnotherCus(tmpCus->name);
				}
			}
			tmpCus = tmpCusNext;
		}

		tmpCusNext = nullptr;
		tmpCus = nullptr;
		return count;
	}

	void printDomain		(int option) 
	{
		if (anotherCus == nullptr || totalCusAnother == 0) {
			return;
		}
		Restaurant::customer *tmpCus = anotherCus;	
		while (tmpCus->next != nullptr) {
			tmpCus = tmpCus->next;
		}	
		while (tmpCus != nullptr) {
			if ((option == 1 && tmpCus->energy > 0) || (option == 2 && tmpCus->energy < 0)) {
				tmpCus->print();
			}
			tmpCus = tmpCus->prev;
		}
		tmpCus = nullptr;
	}

	void printLight			(Restaurant::customer *head, int size, int option)
	// option = 0: in toàn bộ hàng chờ
	// option = 1: in toàn bộ bàn tròn theo cùng chiều kim đồng hồ
	// option = -1: in toàn bộ bàn tròn theo ngược chiều kim đồng hồ
	{
		Restaurant::customer *tmpHead = head;
		for (int i = 0; i < size; i++) {
			tmpHead->print();
			if (option == 0 || option == 1) {
				tmpHead = tmpHead->next;
			}
			else {
				tmpHead = tmpHead->prev;
			}
		}
		tmpHead = nullptr;
	}



	/* for table */
	void push_adjacent					(Restaurant::customer *&head, int &size, string name, int energy, bool clockDirection, int option) 
	// option = 1: push into tableCus;
	// option = 2: push into subList;
	{
		Restaurant::customer *newCus = new Restaurant::customer(name, energy, nullptr, nullptr);	COUNTDELETE++;
		if (size == 0 || head == nullptr) {
			head = newCus;
			head->next = newCus;
			head->prev = newCus;
			size = 1;
			return;
		}
		if (option == 1) {
			if (clockDirection == true) {
				head->next->prev = newCus;
				newCus->next = head->next;
				head->next = newCus;
				newCus->prev = head;
			}
			else {
				head->prev->next = newCus;
				newCus->prev = head->prev;
				head->prev = newCus;
				newCus->next = head;
			}
			head = newCus;
		}
		else if (option == 2) {
			Restaurant::customer *tmpHead = head;
			for (int i = 0; i < size - 1; i++) {
				tmpHead = tmpHead->next;
			}
			tmpHead->next = newCus;
			newCus->prev = tmpHead;
			newCus->next = head;
			head->prev = newCus;
		}
		size++;
	}

	void push_sameClockDirection		(string name, int energy)
	{
		push_adjacent(tableCus, totalCusTable, name, energy, true, 1);
	}

	void push_oppositeClockDirection	(string name, int energy)
	{
		push_adjacent(tableCus, totalCusTable, name, energy, false, 1);
	}

	void push_intopSubList				(string name, int energy)
	{
		return push_adjacent(subList, totalCusSubList, name, energy, true, 2);
	}

	void pop_TableCus 					(const string name)
	{
		Restaurant::customer *tmpCus = tableCus;
		if (totalCusTable == 0 || tableCus == nullptr) {
			tmpCus = nullptr;
			return;
		}
		else {
			for (int i = 0; i < totalCusTable; i++) {
				if (tmpCus->name == name) {
					break;
				}
				tmpCus = tmpCus->next;
			}
		}
				
		if (tmpCus->energy > 0) {
			tableCus = tmpCus->next;
		}
		else if (tmpCus->energy < 0) {
			tableCus = tmpCus->prev;
		}

		tmpCus->prev->next = tmpCus->next;
		tmpCus->next->prev = tmpCus->prev;

		delete tmpCus;	COUNTDELETE--;
		tmpCus = nullptr;

		totalCusTable--;
	}

	int getRES 							(int newEnergy)
	{
		Restaurant::customer *tmpCus = tableCus;
		int RES = newEnergy - tmpCus->energy;

		tmpCus = tmpCus->next;
		for (int i = 1; i < totalCusTable; ++i) {
			if (abs(newEnergy - tmpCus->energy) > abs(RES)) {
				RES = newEnergy - tmpCus->energy;
				tableCus = tmpCus; // Cập nhật tableCus để nó tới vị trí mới cần tìm - vị trí X
			}
			tmpCus = tmpCus->next;
		}
		tmpCus = nullptr;
		return RES;
	}

	void reversal						(Restaurant::customer *oldTableCus, int option)
	// option = 1: sắp xếp các chú thuật sư (các khách có energy > 0)
	// option = 2: sắp xếp các oán linh (các khách có energy < 0)
	{
		if (totalCusTable <= 1) {
			return;
		}

		Restaurant::customer *retrieveNext = oldTableCus; 					int i = 0;
		Restaurant::customer *retrievePrev = oldTableCus->next; 			int j = totalCusTable + 1;

		while (true) {
			if (option == 1) {
				do {
					retrieveNext = retrieveNext->next;
					i++;
				} while (retrieveNext->energy < 0 && i <= totalCusTable);

				do {
					retrievePrev = retrievePrev->prev;
					j--;
				} while (retrievePrev->energy < 0 && j >= 0);
			}
			else if (option == 2) {
				do {
					retrieveNext = retrieveNext->next;
					i++;
				} while (retrieveNext->energy > 0 && i <= totalCusTable);

				do {
					retrievePrev = retrievePrev->prev;
					j--;
				} while (retrievePrev->energy > 0 && j >= 0);
			}

			if (i >= j) {
				return;
			}
			else {
				// Dùng để cập nhật lại TableCus nếu khi TableCus bị hoán đổi vị trí
				if (tableCus->name == retrieveNext->name) {
					tableCus = retrievePrev;
				}
				else if (tableCus->name == retrievePrev->name) {
					tableCus = retrieveNext;
				}
				swap(retrieveNext->energy, retrievePrev->energy);
				swap(retrieveNext->name, retrievePrev->name);
			}
		}
		retrieveNext->next = nullptr; retrieveNext->prev = nullptr;
		retrievePrev->next = nullptr; retrievePrev->prev = nullptr;
	}

	int findSmallestSubList 			(Restaurant::customer *head, int &endIndex)
	{
		Restaurant::customer *tmpHead = head;
		int currentSum = tmpHead->energy;
		int totalMin = currentSum;

		tmpHead = tmpHead->next;
		for (int i = 1; i < 4; i++) {
			currentSum += tmpHead->energy;
			tmpHead = tmpHead->next;
		}
		totalMin = currentSum;
		endIndex = 3;

		for (int i = 4; i < totalCusTable; i++) {
			currentSum += tmpHead->energy;
			if (totalMin >= currentSum) {	// Dấu = là để chọn dãy con dài nhất
				totalMin = currentSum;
				endIndex = i;
			}
			tmpHead = tmpHead->next;
		}

		tmpHead = nullptr;
		return totalMin;
	}

	void unlimited_void 				(Restaurant::customer *&tmpTableCus, int &endIndex)
	{
		int smallestSum = 2147483647;
		int tmpSum = smallestSum;
		int tmpIndex = 0;

		Restaurant::customer *tmpCus = tableCus;
		for (int i = 0; i < totalCusTable; i++) {
			tmpSum = findSmallestSubList(tmpCus, tmpIndex);
			// Nếu tìm được tổng nhỏ hơn thì cập nhật lại tmpTableCus và endIndex
			// Nếu tìm được tổng bằng nhau thì kiểm tra xem độ dài vừa tìm được có dài hơn hoặc bằng độ dài lớn nhất đang có không
			//// Nếu có thì cập nhật
			//// Nếu không thì bỏ qua
			if (tmpSum < smallestSum || (tmpSum == smallestSum && endIndex <= tmpIndex)) {	// Dấu = là để chọn dãy con dài nhất cuối cùng
				smallestSum = tmpSum;
				endIndex = tmpIndex;
				tmpTableCus = tmpCus;
			}
			tmpCus = tmpCus->next;
		}
	}

	customer* findMinSubList 			() 
	{ // Hàm trả về vị trí của khách hàng có energy nhỏ nhất trong chuỗi con
		if (totalCusSubList <= 1) {
			return subList;
		}
		Restaurant::customer *tmpCus = subList;
		Restaurant::customer *res = tmpCus;
		int minEnergy = tmpCus->energy;
		tmpCus = tmpCus->next;
		for (int i = 1; i < totalCusSubList; i++) {
			if (tmpCus->energy < minEnergy) {	// Trả về khách hàng đầu tiên có energy nhỏ nhất nên không lấy dấu =
				minEnergy = tmpCus->energy;
				res = tmpCus;
			}
			tmpCus = tmpCus->next;
		}
		tmpCus = nullptr;
		return res;
	}



	/* for queue */	
	struct attribute
	{
		string name;
		int energy;
		attribute() : name(""), energy(0) {}
		attribute(string na, int en) : name(na), energy(en) {}
	};

	void pushQueue 					(Restaurant::customer *&head, int &size, string name, int energy)
	{
		Restaurant::customer *newCus = new Restaurant::customer(name, energy, nullptr, nullptr); COUNTDELETE++;
		if (size == 0 || head == nullptr) {
			head = newCus;
			head->next = nullptr;
			head->prev = nullptr;
		}
		else {
			Restaurant::customer *tmpHead = head;
			for (int i = 0; i < size - 1; i++) {
				tmpHead = tmpHead->next;
			}
			tmpHead->next = newCus;
			newCus->prev = tmpHead;
		}
		size++;
	}

	void pushFront_Queue			(Restaurant::customer *&head, int &size, string name, int energy)
	{
		Restaurant::customer *newCus = new Restaurant::customer(name, energy, nullptr, nullptr); COUNTDELETE++;
		if (size == 0 || head == nullptr) {
			head = newCus;
			head->next = nullptr;
			head->prev = nullptr;
		}
		else {
			newCus->next = head;
			head->prev = newCus;
			head = newCus;
		}
		size++;
	}

	void push_intoQueueCus 			(string name, int energy)
	// Thao tác này đã được đảm bảo trước rằng vẫn còn không gian để lưu trữ vị khách mới
	{
		if (totalCusQueue >= MAXSIZE) {
			return;
		}
		pushQueue(queueCus, totalCusQueue, name, energy);
	}

	void push_intoAnotherCus 		(string name, int energy)
	// Thao tác này đã được đảm bảo trước rằng vẫn còn không gian để lưu trữ vị khách mới
	{
		return pushQueue(anotherCus, totalCusAnother, name, energy);
	}

	void popQueue 					(Restaurant::customer *&head, int &size, string name)
	{
		Restaurant::customer *tmpHead = head;
		if (size == 0 || head == nullptr) {
			tmpHead = nullptr;
			return;
		}
		// Xóa đầu
		else if (head->name == name) {
			head = head->next;
			if (size != 1) {
				head->prev = nullptr;
			}
		}
		// Xóa giữa và xóa cuối
		else {
			while (tmpHead != nullptr) {
				if (tmpHead->name == name) {
					break;
				}
				tmpHead = tmpHead->next;
			}
			// Khi duyệt qua toàn bộ queue mà vẫn không tìm thấy customer với name cần tìm, thì customer đó không tồn tại
			if (tmpHead == nullptr) {
				return;
			}
			// Nếu tồn tại thì tiến hành xóa như bình thường
			if (tmpHead->prev != nullptr) {
				tmpHead->prev->next = tmpHead->next;
			}
			if (tmpHead->next != nullptr) {
				tmpHead->next->prev = tmpHead->prev;
			}
		}
		delete tmpHead; COUNTDELETE--;
		tmpHead = nullptr;
		size--;
	}

	void pop_QueueCus				(const string name)
	{
		popQueue(queueCus, totalCusQueue, name);
	}

	void pop_AnotherCus				(const string name)
	{
		popQueue(anotherCus, totalCusAnother, name);
	}

	attribute popFront_QueueCus		()
	{
		attribute res(queueCus->name, queueCus->energy);
		pop_QueueCus(queueCus->name);
		return res;
	}

	customer* getCusAt_inQueue 		(int startIndex, int index)
	{ // startIndex: vị trí index bắt đầu nhảy số
		if (index < 0 || index >= totalCusQueue) {
			return nullptr;
		}
		Restaurant::customer *tmpCus = queueCus;
		for (int i = 0; i < index + startIndex; i++) {
			tmpCus = tmpCus->next;
		}
		return tmpCus;
	}

	void swapCus 					(int startIndex, int index1, int index2)
	{
		Restaurant::customer *cus1 = getCusAt_inQueue(startIndex, index1);
		Restaurant::customer *cus2 = getCusAt_inQueue(startIndex, index2);

		swap(cus1->name, cus2->name);
		swap(cus1->energy, cus2->energy);
	}

	bool comeEarlier 				(int startIndex, int index1, int index2) 
	{ // Trả về true nếu index1 đến sớm hơn index2.
		Restaurant::customer *cus1 = getCusAt_inQueue(startIndex, index1);
		Restaurant::customer *cus2 = getCusAt_inQueue(startIndex, index2);

		Restaurant::customer *tmpCus = anotherCus;
		for (int i = 0; i < totalCusAnother; i++) {
			if (cus1->name == tmpCus->name) {
				return true;
			}
			if (cus2->name == tmpCus->name) {
				return false;
			}
			tmpCus = tmpCus->next;
		}
		return false;
	}

	bool Energy_greaterThan 		(int startIndex, int index1, int index2) 
	{ // Trả về true nếu khách ở index1 có energy lớn hơn khách ở index2
		Restaurant::customer *cus1 = getCusAt_inQueue(startIndex, index1);
		Restaurant::customer *cus2 = getCusAt_inQueue(startIndex, index2);

		if (abs(cus1->energy) > abs(cus2->energy)) {
			return true;
		}
		// Trong trường hợp 2 energy (dưới dạng KHÔNG TRỊ TUYỆT ĐỐI) bằng nhau, thì khách nào đến trước sẽ được xem là có energy lớn hơn
		if (abs(cus1->energy) == abs(cus2->energy)) {
			if (comeEarlier(startIndex, index1, index2)) {
				return true;
			}
		}

		cus1 = nullptr;
		cus2 = nullptr;
		return false;
	}

	int insertionSort 				(int startIndex, const int size, int interval)
	{ // InsertionSort bổ trợ cho shellSort, trả về số lần chuyển đổi vị trí
		int count_swappedFrequency = 0;
		for (int i = interval; i < size; i += interval) {
			for (int j = i - interval; j >= 0; j -= interval) {
				if (Energy_greaterThan(startIndex, j, j + interval)) {
					break;
				}
				swapCus(startIndex, j, j + interval);
				count_swappedFrequency++;
			}
		}
		return count_swappedFrequency;
	}

	int shellSort					(const int size) 
	{ // ShellSort sắp xếp hàng chờ, trả về số lần chuyển đổi vị trí;
		int count_swappedFrequency = 0;
		for (int interval = size/2; interval > 2; interval /= 2) {
			for (int step = 0; step < interval; step++) {
				count_swappedFrequency += insertionSort(step, size - step, interval); // Sự có mặt của Step là lí do vì sao phải có đối số startIndex ở các hàm phía trên
			}
		}
		count_swappedFrequency += insertionSort(0, size, 1);
		return count_swappedFrequency;
	}

	int getIndex_maxEnergy			()
	{
		// Không cần kiểm tra điều kiện xem hàng chờ có khách hay không, vì đã làm điều đó trong hàm PURPLE (trước khi gọi hàm này)
		Restaurant::customer *tmpCus = queueCus;
		int index = 0;
		int maxEnergy = abs(tmpCus->energy);
		tmpCus = tmpCus->next;
		for (int i = 1; i < totalCusQueue; i++) {
			// tìm vị trí index của khách hàng có trị tuyệt đối energy cao nhất trong hàng chờ
			if ((maxEnergy < abs(tmpCus->energy)) || (maxEnergy == abs(tmpCus->energy) && comeEarlier(0, index, i))) {
				maxEnergy = abs(tmpCus->energy);
				index = i;
			}
			tmpCus = tmpCus->next;
		}
		tmpCus = nullptr;
		return index;
	}



	///////////////////////////////////////////////////////////		FUCNTION RED	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void RED(string name, int energy) override
	{
		if (energy == 0 || is_identicalName(name) || totalCusQueue >= MAXSIZE) {		// Nếu (không là chú thuật sư và cũng không là oán linh) || (tên bị trùng với 1 vị khách hiện hữu) || (số khách trong hàng chờ đã tối đa) -> return;
			return;
		}										
		else if (totalCusTable == MAXSIZE) {											// Nếu số lượng khách trong bàn tròn đã đạt đến số lượng tối đa MAXSIZE, thì push vào hàng chờ queueCus
			push_intoQueueCus(name, energy);
		}
		else if (totalCusTable == 0) {													// Nếu là vị khách đầu tiên -> random vị trí bất kì trong DoublyCircularLinkedList (bàn tròn)
			push_sameClockDirection(name, energy);
		}
		// Các vị khách tiếp theo sẽ được xếp dựa theo energy
		else if (totalCusTable >= MAXSIZE/2) {											// Khi số lượng khách >= MAXSIZE/2 :
			int RES = getRES(energy);
			if (RES < 0) {
				push_oppositeClockDirection(name, energy);
			}
			else {
				push_sameClockDirection(name, energy);
			}
		}													
		else if (totalCusTable < MAXSIZE/2) {											// Khi số lượng khách <  MAXSIZE/2 :
			if (energy >= tableCus->energy) {
				push_sameClockDirection(name, energy);
			}
			else {
				push_oppositeClockDirection(name, energy);
			}
		}
		// Nếu các trong anotherCus chưa có khách với tên 'name' thì mới push vào
		if (!checkExist(anotherCus, totalCusAnother, name)) {
			push_intoAnotherCus(name, energy);
		}
	}





	///////////////////////////////////////////////////////////		FUCNTION BLUE	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void BLUE(int num) override
	{
		if (totalCusTable == 0 || num == 0) {
			return;
		}
		// Nếu (num >= số khách hành hiện hữu ) hoặc (num > MAXSIZE - kích thước tối đa của nhà hàng) thì xóa hết toàn bộ khách hàng -> cho num = totalCusTable
		if (num >= totalCusTable || num > MAXSIZE) {
			num = totalCusTable;
		}

		Restaurant::customer *tmpCus = anotherCus;
		for (int i = 0; i < num; i++) {
			while (!checkExistTable(tmpCus->name)) {
				tmpCus = tmpCus->next;
			}
			pop_TableCus(tmpCus->name); // xóa khách hàng vừa tìm được ra khỏi bàn tròn
			pop_AnotherCus(tmpCus->name);
			tmpCus = anotherCus;
		}
		tmpCus = nullptr;

		// Sau khi đuổi khách, còn vị trí trống trong bàn tròn, nếu trong hàng chờ có khách, thì đưa khách đó từ hàng chờ vào bàn tròn
		for (int i = 0; i < num; i++) {
			if (totalCusQueue == 0) {	// Bàn còn trống, nhưng hàng chờ đã rỗng, thì không thực hiện nữa
				break;
			}
			attribute newCus = popFront_QueueCus();
			RED(newCus.name, newCus.energy);
		}
	}




	
	///////////////////////////////////////////////////////////		FUCNTION PURPLE	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void PURPLE() override
	{
		if (totalCusQueue == 0 || queueCus == nullptr) { 
			return;
		}
		int indexOfMaxEnergy = getIndex_maxEnergy();					// Tìm vị trí có trị tuyệt đối ENERGY cao nhất - vị trí M // Dòng code này đã đảm bảo bằng trong hàng chờ có ít nhất 1 khách để truy cập
		int count_swappedFrequency = shellSort(indexOfMaxEnergy + 1);	// Sắp xếp từ đầu đến vị trí M

		BLUE(count_swappedFrequency % MAXSIZE);							// Thực hiện thuật thức BLUE với đối số là {số lần chuyển đổi) % MAXSIZE}
	}





	///////////////////////////////////////////////////////////		FUCNTION REVERSAL	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void REVERSAL() override
	{
		Restaurant::customer *oldTableCus = tableCus;
		reversal(oldTableCus, 1);
		reversal(oldTableCus, 2);
		oldTableCus = nullptr;
	}





	///////////////////////////////////////////////////////////		FUCNTION UNLIMITED_VOID	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void UNLIMITED_VOID() override
	{
		deleteCus(subList, totalCusSubList, 1);
		
		if (totalCusTable < 4) {
			return;
		}
		int endIndex = 0;
		Restaurant::customer *tmpTableCus = tableCus;
		unlimited_void(tmpTableCus, endIndex);

		// Tạo chuỗi con mới theo yêu cầu
		for (int i = 0; i <= endIndex; i++) {
			push_intopSubList(tmpTableCus->name, tmpTableCus->energy);
			tmpTableCus = tmpTableCus->next;
		}
		// In chuỗi con mới theo yêu cầu
		Restaurant::customer *marked_smallest = findMinSubList();
		for (int i = 0; i < totalCusSubList; i++) {
			marked_smallest->print();
			marked_smallest = marked_smallest->next;
		}
		tmpTableCus = nullptr;
		marked_smallest = nullptr;
	}





	///////////////////////////////////////////////////////////		FUCNTION DOMAIN_EXPANSION	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void DOMAIN_EXPANSION() override
	{
		if ((totalCusTable == 0 && totalCusQueue == 0) || (totalCusAnother == 1)) {
			return;
		}

		int sumPostive = 0;
		int sum = 0;
		sumEnergy(sumPostive, sum);
		int count;	// Số lượng khách bị kick trong bàn tròn

		if (sumPostive >= abs(sum)) {
			printDomain(2);
			count = kickCus(2);
		}
		else if (sumPostive < abs(sum)) {
			printDomain(1);
			count = kickCus(1);
		}

		for (int i = 0; i < count; i++) {
			if (totalCusQueue == 0) {	// Bàn còn trống, nhưng hàng chờ đã rỗng, thì không thực hiện nữa
				break;
			}
			attribute newCus = popFront_QueueCus();
			RED(newCus.name, newCus.energy);
		}
	}





	///////////////////////////////////////////////////////////		FUCNTION LIGHT	  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\.
	void LIGHT(int num) override
	{
		if (num > 0) {
			printLight(tableCus, totalCusTable, 1);
		}
		else if (num == 0) {
			printLight(queueCus, totalCusQueue, 0);
		}
		else {
			printLight(tableCus, totalCusTable, -1);
		}
	}
};
