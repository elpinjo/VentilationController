#ifndef VENTILATION_HTTPREQUEST_H
#define VENTILATION_HTTPREQUEST_H

#include <map>
#include <list>
#include <Arduino.h>

class HTTPRequest {

    public:
        HTTPRequest(String rawRequest);
        ~HTTPRequest();
        String getMethod();
        String getResourcePath();
        std::list<String> getParameters();
        String getParameterValue(String aParameter);
        std::list<String> getHeaders();
        String getHeaderValue(String aHeader);
        String getBody();
    private:
        void parseMethod(String methodString);
        void parseHeader(String headerString);
        void parseParameters();
        void parseParamString(String paramString);
        String getLine(String& aLine,char aSeparator, String& result);

        std::map<String, String> parameters;
        std::map<String, String> headers;
        String method;
        String resourcePath;
        String body;
};

#endif  