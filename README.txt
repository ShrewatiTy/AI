Troubleshooting AI Website using C

Files:
- index.html: website form.
- styles.css: page styling.
- cgi-bin/troubleshooter.c: C CGI backend for processing the form.

To use:
1. Place the TroubleshootingAI folder in a web server that supports CGI, such as Apache or Nginx with CGI enabled.
2. Compile the C source to a CGI executable:
   gcc -o cgi-bin/troubleshooter.cgi cgi-bin/troubleshooter.c
3. Ensure the server permits execution of the CGI program.
4. Open index.html in your browser or navigate to the site through the server.

Notes:
- This example is a simple rule-based troubleshooting assistant.
- It is intended for learning and demonstration, not production use.
