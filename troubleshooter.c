#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 2048
#define MAX_ESCAPED 4096

static void decode_url(char *src) {
    char *dst = src;
    while (*src) {
        if (*src == '+') {
            *dst++ = ' ';
        } else if (*src == '%' && src[1] && src[2]) {
            char hex[3] = {src[1], src[2], '\0'};
            *dst++ = (char) strtol(hex, NULL, 16);
            src += 2;
        } else {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

static int starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}

static void parse_field(const char *data, const char *key, char *output, size_t outSize) {
    output[0] = '\0';
    char *match = strstr(data, key);
    if (!match) return;
    match += strlen(key);
    if (*match != '=') return;
    match++;
    char *end = strchr(match, '&');
    size_t len = end ? (size_t)(end - match) : strlen(match);
    if (len >= outSize) len = outSize - 1;
    strncpy(output, match, len);
    output[len] = '\0';
    decode_url(output);
}

static void print_response(const char *category, const char *description) {
    printf("Content-Type: text/html\r\n\r\n");
    printf("<!DOCTYPE html><html lang=\"en\"><head>");
    printf("<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    printf("<title>Troubleshooting Result</title>");
    printf("<style>body{font-family:Arial,sans-serif;background:#0b1220;color:#e2e8f0;padding:24px;} .card{max-width:800px;margin:auto;padding:24px;background:#111827;border-radius:16px;box-shadow:0 12px 28px rgba(0,0,0,.35);}h1{color:#60a5fa;}p{line-height:1.7;}pre{white-space:pre-wrap;background:#111827;border:1px solid #334155;padding:16px;border-radius:12px;color:#cbd5e1;}</style>");
    printf("</head><body><div class=\"card\"><h1>Troubleshooting AI Assistant</h1>\n");
    printf("<p><strong>Category:</strong> %s</p>\n", category[0] ? category : "Not provided");
    printf("<p><strong>Description:</strong></p><pre>%s</pre>\n", description[0] ? description : "None");

    if (strcmp(category, "installation") == 0) {
        printf("<h2>Installation or setup guidance</h2>");
        printf("<p>Check your environment, verify the AI library or runtime is installed, and confirm the correct model or API key is configured.</p>");
        printf("<ul><li>Reinstall or update dependencies.</li><li>Verify network access if the AI requires cloud connectivity.</li><li>Ensure configuration files contain valid paths and credentials.</li></ul>");
    } else if (strcmp(category, "performance") == 0) {
        printf("<h2>Performance troubleshooting</h2>");
        printf("<p>Look for long-running operations, large model loads, or insufficient memory.</p>");
        printf("<ul><li>Reduce batch sizes or request sizes.</li><li>Reload models with optimized precision if available.</li><li>Monitor CPU/GPU usage and available memory.</li></ul>");
    } else if (strcmp(category, "accuracy") == 0) {
        printf("<h2>Output quality advice</h2>");
        printf("<p>Improve prompts, provide clearer examples, and validate your training or fine-tuning data.</p>");
        printf("<ul><li>Use more explicit instructions.</li><li>Provide examples to shape behavior.</li><li>Check for biases or unexpected tokens in model output.</li></ul>");
    } else if (strcmp(category, "integration") == 0) {
        printf("<h2>Integration and API troubleshooting</h2>");
        printf("<p>Verify request/response formats, authorization headers, and endpoint URLs.</p>");
        printf("<ul><li>Confirm API keys are correct and not expired.</li><li>Check response codes and error messages.</li><li>Make a minimal test call first.</li></ul>");
    } else {
        printf("<h2>General troubleshooting</h2>");
        printf("<p>Collect logs, reproduce the issue with a minimal example, and compare the behavior to expected results.</p>");
        printf("<ul><li>Use clear, concise test cases.</li><li>Search for similar errors and known issues.</li><li>Document the exact steps to reproduce the problem.</li></ul>");
    }

    printf("<p><a href=\"../index.html\" style=\"color:#93c5fd;\">Return to form</a></p>");
    printf("</div></body></html>\n");
}

int main(void) {
    char input[MAX_INPUT] = "";
    const char *method = getenv("REQUEST_METHOD");
    if (!method) {
        printf("Content-Type: text/plain\r\n\r\n");
        printf("Error: REQUEST_METHOD not specified.\n");
        return 1;
    }

    if (strcmp(method, "POST") == 0) {
        const char *length = getenv("CONTENT_LENGTH");
        if (length) {
            int len = atoi(length);
            if (len > 0 && len < MAX_INPUT) {
                fread(input, 1, len, stdin);
                input[len] = '\0';
            }
        }
    }

    char problemType[128];
    char description[1024];
    parse_field(input, "problemType", problemType, sizeof(problemType));
    parse_field(input, "description", description, sizeof(description));
    print_response(problemType, description);
    return 0;
}
