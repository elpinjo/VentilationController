#include "HTTPRequest.hpp"
#include <iterator>

using namespace std;

//=======================================================================
// Public methods
//=======================================================================
HTTPRequest::HTTPRequest(String rawRequest){

    String line;
    bool isBody = false;

    String remainder = getLine(rawRequest, '\n', line);

    parseMethod(line);

    body = "";

    while (remainder.length() > 0) {
        remainder = getLine(remainder, '\n', line);

        if (line.equals("") || line.equals("\r")) {

            isBody = true;
        }

        if (isBody) {
            body.concat(line);
        } else {
            parseHeader(line);
        }
    }
}

HTTPRequest::~HTTPRequest() {

    for(std::map<String,String>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
        parameters.erase(it);
    }

    for(std::map<String,String>::iterator it = headers.begin(); it != headers.end(); ++it) {
        headers.erase(it);
    }

    body = "";
    resourcePath = "";
    method = "";
}

String HTTPRequest::getMethod(){
    return method;
}

String HTTPRequest::getResourcePath(){
    return resourcePath;
}

list<String> HTTPRequest::getParameters(){
    list<String> paramKeys;

    for(std::map<String,String>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
        paramKeys.push_back(it->first);
    }
    
    return paramKeys;
}

String HTTPRequest::getParameterValue(String aParameter){

    std::map<String,String>::iterator iter = parameters.find(aParameter);
    
    if (iter == parameters.end()) {
        return "";
    } else {
        return iter->second;
    }
}

list<String> HTTPRequest::getHeaders(){
    list<String> headerKeys;

    for(std::map<String,String>::iterator it = headers.begin(); it != headers.end(); ++it) {
        headerKeys.push_back(it->first);
    }
    
    return headerKeys;
}

String HTTPRequest::getHeaderValue(String aHeader){
    std::map<String,String>::iterator iter = headers.find(aHeader);
    
    if (iter == headers.end()) {
        return "";
    } else {
        return iter->second;
    }
}

String HTTPRequest::getBody(){
    return body;
}

//=======================================================================
// Private methods
//=======================================================================

void HTTPRequest::parseMethod(String methodString) {

    String remainder;
    remainder = getLine(methodString, ' ', method);
    remainder = getLine(remainder, ' ', resourcePath);
    parseParameters();
}

void HTTPRequest::parseParameters() {

    int index;
    if ((index = resourcePath.indexOf("?")) > 0) {

        String paramString = resourcePath.substring(index+1);
        resourcePath = resourcePath.substring(0,index);

        parseParamString(paramString);

    }

}

void HTTPRequest::parseParamString(String paramString) {
    
    int index = paramString.indexOf("=");
    String paramName = paramString.substring(0, index), paramValue;
    
    paramString = paramString.substring(index + 1);
    
    index = paramString.indexOf("&");
    if (index > 0) {
        paramValue = paramString.substring(0, index);
        paramString = paramString.substring(index + 1);

        parameters[paramName] = paramValue;

        parseParamString(paramString);
    } else {
        paramValue = paramString;

        parameters[paramName] = paramValue;
    }
}

void HTTPRequest::parseHeader(String headerString) {
    
    String header, value;

    int index = headerString.indexOf(":");

    if (index > 0) {
        header = headerString.substring(0, index);

        if (headerString.charAt(index+1)==' ') {
            value = headerString.substring(index+2);
        } else {
            value = headerString.substring(index+1);
        }

        headers[header] = value;
    }
}

String HTTPRequest::getLine(String& aLine,char aSeparator, String& result) {

    int pos = aLine.indexOf(aSeparator);
    if (pos > 0) {
        result = aLine.substring(0, pos);
    } else {
        result = aLine;
        return "";
    }
    return aLine.substring(pos+1);
}