int f() {
  return 5 * 10 + 4;
}

string g(int a) {
    if (a == 5) 
	return "hello";
    else
	return "world";
}

void main() {
  string s;
  s = g(4 * 5 / 2);
}