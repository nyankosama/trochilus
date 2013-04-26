#!/usr/bin/python 
import cgi 
def main():
    print "Content-type: text/html\n"
    form = cgi.FieldStorage() # parse query
    if form.has_key("username") and form["username"].value != "":
        print "<h1>Hello", form["username"].value, "</h1>"
    else:
        print "<h1>Error! Please enter first name.</h1>" 
main()
