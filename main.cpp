#include <iostream>
#include <fstream>
#include <string>

#include "main.h"
#include "HTTPDownloader.h"
#include <libxml++/libxml++.h>

// Prototype Declarations
void print_indentation(unsigned int indentation);
void print_node(const xmlpp::Node* node, unsigned int indentation = 0);

int main(int, char**) {
    std::cout << "Hello, world!\n";
    std::cout << "My name is pacho" << std::endl;

    for(int i=0; i<MAX_ITERATIONS; i++)
    {
        std::cout << "Iteration #" << i << std::endl;
    }

    HTTPDownloader downloader;
    std::string html_content = downloader.download("http://www.mangapanda.com/shingeki-no-kyojin");
    // Save downloaded HTML to file
    std::ofstream out("output.html");
    out << html_content;
    out.close();

    // Convert HTML to XML
    std::string html_clean_xml = downloader.CleanHTML(html_content);

    // Save XML content to file
    std::ofstream out2("output.xml");
    out2 << html_clean_xml;
    out2.close();

    // // Let's parse xml
    xmlpp::DomParser doc;
    doc.parse_memory(html_clean_xml);
    xmlpp::Document* document = doc.get_document();
    xmlpp::Element* root = document->get_root_node();

    xmlpp::NodeSet elemns = root->find("descendant-or-self::*[@href]");
    // xmlpp::NodeSet elemns = root->find("//div[@id='latestchapters']/ul/li[1]/a");
    std::cout << elemns[0]->get_line() << std::endl;
    std::cout << elemns[0]->get_name() << std::endl;
    std::cout << elemns.size() << std::endl;
    
    // Print node and all children, grandchildren, etc. recursively
    print_node(elemns[1]);
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
