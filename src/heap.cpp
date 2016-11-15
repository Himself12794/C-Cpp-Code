#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

struct entry {
	int e;
	int depth;
	bool is_right_child;
	bool has_right_child;
};

template<typename T> 
void print_v(vector<T> v) {
	
	cout << "{";
	for (unsigned int i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i != v.size() - 1) cout << ", ";
		else cout << "}" << endl;
	}
	
}

string rstrip(string &str) {
	size_t l = str.find_last_not_of(' ');
	
	return str.substr(0, l + 1);
}

string strip(string &str) {
	size_t f = str.find_first_not_of(' ');
	size_t l = str.find_last_not_of(' ');
	
	return str.substr(f, (l - f + 1));
}

string center(string &str, size_t n) {
	if (str.length() < n) {
		size_t f = str.find_first_not_of(' ');
		size_t l = n - str.find_last_not_of(' ');
		
		size_t i = 0;
		bool dl = l > f ? false : true; 
		while (i < f + l - 1) {
			if (dl) str.insert(0, " ");
			else str.insert(str.length(), " ");
			dl = !dl;
			i++;
		}
	} else return str;
	return str;
}

vector<int> read_file(char const *fname) {
    
	list<int> dl;
	
	fstream input;
	input.open(fname);
	
	int a;
	while (input >> a) {
		dl.push_back(a);
	}
	
	vector<int> dv{ dl.begin(), dl.end() };
	
    return dv;
}

int parent_index(int i) {
    return (i - 1) / 2;
}

int left_child_index(int i) {
    return 2 * i + 1;
}

int right_child_index(int i) {
    return 2 * i + 2;
}

void up_heap(vector<int> &data, int n) {
    if (n == 0) {
        return;
	}
    int p = parent_index(n);
    if (data[p] >= data[n]) {
        return;
    } else {
		swap(data[p], data[n]);
        up_heap(data, p);
	}
}

void build_heap(vector<int> &data) {
    int n = data.size();
	for (int i = 0; i < n; ++i) {
        up_heap(data, i);
	}
}

void down_heap(vector<int> &data, int n, int stop) {
    int left = left_child_index(n);
    int right = right_child_index(n);
	
    if (left >= stop) {
        // case 1: already a leave
        return;
    } else if (left < stop && right >= stop) {
        // case 2: only have a left child
        if (data[n] < data[left]) {
			swap(data[n], data[left]);
            down_heap(data, left, stop);
        } else return;
    } else {
        // case 3: has both left and right children
        if (data[n] >= data[left] && data[n] >= data[right]) {
            return;
        } else {
            int largest_child = left;
            if (data[left] < data[right]) largest_child = right;
			
			swap(data[n], data[largest_child]);
            down_heap(data, largest_child, stop);
		}
	}
}

void sort_heap(vector<int> &data) {
    int n = data.size();
	for (int i = n - 1; i > 0; --i) {
		swap(data[0], data[i]);
        down_heap(data, 0, i);
	}
}

template<typename T> 
vector<T> conc_vect(vector<T> a, vector<T> b) {
	vector<T> ab;
	ab.reserve(a.size() + b.size());
	ab.insert(ab.end(), a.begin(), a.end());
	ab.insert(ab.end(), b.begin(), b.end());
	return ab;
}

vector<entry> inorder(vector<int> &data, int n = 0, int depth = 0) {
    int nn = data.size();
    if (n >= nn) {
		vector<entry> out;
        return out;
	}
    int left = left_child_index(n);
    int right = right_child_index(n);
    bool is_right_child = (n % 2) == 0; // only exception is for the root
    bool has_right_child = (n * 2 + 2) < nn;
	
	entry e = { data[n], depth, is_right_child, has_right_child};
	vector<entry> a(1, e);
	vector<entry> b = conc_vect(inorder(data, left, depth+1), a);
	return conc_vect(b, inorder(data, right, depth+1));
}

string update_bars(string bars, int depth, bool is_right_child, bool has_right_child) {
    int location1 = 5 * depth - 4; // location for level depth - 1
    int location2 = location1 + 5; // location for level depth
    if (bars.length() < (unsigned)location2) {
        bars = bars + string(location2 - bars.length(), ' ');
	}
    if (depth == 0) {
        // this is the root, no bar to remove
        
    } else if (is_right_child) {
        // remove the bar at level depth - 1
        bars.replace(location1, 1, " ");
    } else {
        // add a bar at level depth - 1
        bars.replace(location1, 1, "|");
	}
    if (!has_right_child) {
        // remove the bar at level depth, since no one else will remove it.
        bars.replace(location2, 1, " ");
	}
    return rstrip(bars);
}


string draw_tree(vector<int> data) {
    string bars = "";
    list<string> results;
    vector<entry> items = inorder(data);
	for (unsigned int i = 0; i < items.size(); ++i) {
		
		int value = items[i].e;
		int depth = items[i].depth;
		bool is_right_child = items[i].is_right_child;
		bool has_right_child = items[i].has_right_child;
		
        results.push_back(bars);
        string value_str = to_string(value);
		value_str = center(value_str, 3);
		
        if (!is_right_child) {
            bars = update_bars(bars, depth, is_right_child, has_right_child);
		}
        if (depth == 0) results.push_back(value_str);
        else {
            int bar_prefix = (depth-1) * 5;
			string temp = bars.substr(0, bar_prefix);
			temp += " +---" + value_str;
            results.push_back(temp);
		}
        if (is_right_child) {
			bars = update_bars (bars, depth, is_right_child, has_right_child);
		}
        results.push_back(bars);
	}
    results.reverse();
	string temp = "";
	for (list<string>::const_iterator i = results.begin(); i != results.end(); ++i) {
		temp += *i + "\n";
	}
    return temp;
}

string sort_data(vector<int> &data) {
    build_heap(data);
    string result = draw_tree(data);
    sort_heap(data);
    return result;
}

bool verify_sorted(vector<int> data) {
    int n = data.size();
    if (n < 2) {
        return true;
    } else {
		for (int i = 1; i < n; ++i) {
            if (data[i-1] > data[i]) return false;
		}
        return true;
	}

}

void process_data(vector<int> data) {
	
	string heap = sort_data(data);
	if (!verify_sorted(data)) {
		cout << "***** Warning: Data not sorted. *****" << endl;
	}
	cout << heap << endl;
	print_v(data);
}

int heap_main(int n, char **args) {
	if (n <= 1) cout << "Usage: heap <infile>" << endl;
	else if (n > 1) process_data(read_file("data.txt"));
	return 0;
}
