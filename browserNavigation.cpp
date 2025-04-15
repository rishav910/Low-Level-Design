#include <bits/stdc++.h>
using namespace std;
/*
    Browser
    Properties:
    - currentPage
    Methods:
    + visit()
    + navigateForward()
    + navigateBackward()
    + getCurrentPage()
*/
class Browser {
    string currentPage;
    stack<string>forward;
    stack<string>backward;
    public:
    // Constructor - initialize with 'home' as current page
    Browser():currentPage("home"){}
    
    void visit(string url) {
        // When visited, last page -> Backward stack
        backward.push(currentPage);
        // Empty the forward stack because new page visited
        while(!forward.empty()) {
            forward.pop();
        }
        currentPage = url;
        cout<<"Visited page: "<<currentPage<<endl;
    }
    void navigateForward() {
        if(forward.empty()) {
            cout<<"Can't move forward"<<endl;
            return;
        }
        // When moved forward, last page -> Backward stack
        backward.push(currentPage);
        currentPage = forward.top();
        forward.pop();
        cout<<"Went forward into "<<currentPage<<" page"<<endl;
    }
    void navigateBackward() {
        if(backward.empty()) {
            cout<<"Can't move backward"<<endl;
            return;
        }
        forward.push(currentPage);
        currentPage = backward.top();
        backward.pop();
        cout<<"Went back into "<<currentPage<<" page"<<endl;
    }
    string getCurrentPage() {
        return currentPage;
    }
};
int main() {
    Browser *browser = new Browser;
    
    browser->visit("Google");
    browser->visit("Yahoo");
    browser->visit("Facebook");
    browser->visit("Leetcode");
    browser->visit("GeeksForGeeks");
    
    cout<<"Current page: "<<browser->getCurrentPage()<<endl;
    browser->navigateBackward();
    browser->navigateBackward();
    browser->navigateForward();
}