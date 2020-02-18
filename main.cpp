#include <iostream>
#include <fstream>
#include <string>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

#define HEADER_ACCEPT "Accept:text/html,application/xhtml+xml,application/xml"
#define HEADER_USER_AGENT "User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.70 Safari/537.17"

// Prototype Declarations
void print_indentation(unsigned int indentation);
void print_node(const xmlpp::Node* node, unsigned int indentation = 0);

int main(int, char**) {
  std::string url = "http://www.mangapanda.com/shingeki-no-kyojin";
  curlpp::Easy request;

  // Specify the URL
  request.setOpt(curlpp::options::Url(url));
 
  // Specify some headers
  std::list<std::string> headers;
  headers.push_back(HEADER_ACCEPT);
  headers.push_back(HEADER_USER_AGENT);
  request.setOpt(new curlpp::options::HttpHeader(headers));
  request.setOpt(new curlpp::options::FollowLocation(true));
 
  // Configure curlpp to use stream
  std::ostringstream responseStream;
  curlpp::options::WriteStream streamWriter(&responseStream);
  request.setOpt(streamWriter);
 
  // Collect response
  request.perform();
  std::string re = responseStream.str();
  // Save downloaded HTML to file
  std::ofstream out("output.html");
  out << re;
  out.close();

  // Parse HTML and create a DOM tree
  xmlDoc* doc = htmlReadDoc((xmlChar*)re.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
 
  // Encapsulate raw libxml document in a libxml++ wrapper
  xmlNode* r = xmlDocGetRootElement(doc);
  xmlpp::Element* root = new xmlpp::Element(r);
 
  // Grab the IP address
  std::string xpath = "//div[@id='latestchapters']/ul/li[1]/a/text()";
  auto elements = root->find(xpath);
  std::cout << "Size of elements: " << elements.size() << std::endl;
  
  std::string chapter_info = dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content();
  std::cout << "The latest Chapter is: " << chapter_info << std::endl;
  std::cout << "Chapter number: " << chapter_info.substr(19,chapter_info.size()) << std::endl;

  // Print node and all children, grandchildren, etc. recursively
  print_node(elements[0]);
 
  delete root;
  xmlFreeDoc(doc);
 
  return 0;
}

void print_indentation(unsigned int indentation)
{
  for(unsigned int i = 0; i < indentation; ++i)
    std::cout << " ";
}

void print_node(const xmlpp::Node* node, unsigned int indentation /*= 0*/)
{
  std::cout << std::endl; //Separate nodes by an empty line.
  
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
    return;
    
  Glib::ustring nodename = node->get_name();

  if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
  {
    print_indentation(indentation);
    std::cout << "Node name = " << node->get_name() << std::endl;
    std::cout << "Node name = " << nodename << std::endl;
  }
  else if(nodeText) //Let's say when it's text. - e.g. let's say what that white space is.
  {
    print_indentation(indentation);
    std::cout << "Text Node" << std::endl;
  }

  //Treat the various node types differently: 
  if(nodeText)
  {
    print_indentation(indentation);
    std::cout << "text = \"" << nodeText->get_content() << "\"" << std::endl;
  }
  else if(nodeComment)
  {
    print_indentation(indentation);
    std::cout << "comment = " << nodeComment->get_content() << std::endl;
  }
  else if(nodeContent)
  {
    print_indentation(indentation);
    std::cout << "content = " << nodeContent->get_content() << std::endl;
  }
  else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:

    //line() works only for ElementNodes.
    print_indentation(indentation);
    std::cout << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
      const xmlpp::Attribute* attribute = *iter;
      print_indentation(indentation);
      std::cout << "  Attribute " << attribute->get_name() << " = " << attribute->get_value() << std::endl;
    }

    const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
    if(attribute)
    {
      std::cout << "title found: =" << attribute->get_value() << std::endl;
      
    }
  }
    
  if(!nodeContent)
  {
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
    {
      print_node(*iter, indentation + 2); //recursive
    }
  }
}
