using Exercise_DataSerialization.Models;
using System.Xml; // XmlWriter
using System.Text.Json; // Utf8JsonWriter, JsonWriterOptions

namespace Exercise_DataSerialization;

public class Serializers
{
    public delegate void WriteXmlDataDelegate(string name, string? value);

    public static void XmlSerilizer(IQueryable<Category> categories, bool useAttributes = false)
    {
        string xmlFileName = $"xml-useAttribues-{useAttributes.ToString()}.xml";

        using var fileStream = File.Create(Path.Combine(Environment.CurrentDirectory, xmlFileName));
        using var xmlWriter = XmlWriter.Create(fileStream, new XmlWriterSettings { Indent = true });

        WriteXmlDataDelegate writeXmlDelegate;

        if (useAttributes)
            writeXmlDelegate = xmlWriter.WriteAttributeString;
        else
            writeXmlDelegate = xmlWriter.WriteElementString;

        // Start XML staff
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("Categories"); // categories

        // XML body
        foreach (var cat in categories)
        {
            xmlWriter.WriteStartElement("Category"); // catorgory
            writeXmlDelegate("Id", cat.CategoryId.ToString());
            writeXmlDelegate("Name", cat.CategoryName);
            writeXmlDelegate("Description", cat.Description);
            writeXmlDelegate("Count", cat.Products.Count.ToString());

            xmlWriter.WriteStartElement("Products");
            foreach (var prod in cat.Products)
            {
                xmlWriter.WriteStartElement("Product");

                writeXmlDelegate("Id", prod.ProductId.ToString());
                writeXmlDelegate("Name", prod.ProductName);
                writeXmlDelegate("Cost", prod.UnitPrice == null ? "0" : prod.UnitPrice.ToString());
                writeXmlDelegate("Stock", prod.UnitsInStock == null ? "0" : prod.UnitsInStock.ToString());
                writeXmlDelegate("Discontinued", prod.Discontinued.ToString());

                xmlWriter.WriteEndElement(); // prod
            }
            xmlWriter.WriteEndElement(); // Products
            xmlWriter.WriteEndElement(); // cat
        }

        // End XML staff
        xmlWriter.WriteEndElement(); // categories
        xmlWriter.WriteEndDocument();
    }

    public static void JsonSerilizer(IQueryable<Category> categories)
    {
        string jsonFileName = $"jsonSerialized.json";

        using var fileStream = File.Create(Path.Combine(Environment.CurrentDirectory, jsonFileName));
        using var jsonWriter = new Utf8JsonWriter(fileStream, new JsonWriterOptions { Indented = true });

        // JSON start of the file staff
        jsonWriter.WriteStartObject();
        jsonWriter.WriteStartArray("Categories");

        foreach (var cat in categories)
        {
            jsonWriter.WriteStartObject();
            jsonWriter.WriteNumber("Id", cat.CategoryId);
            jsonWriter.WriteString("Name", cat.CategoryName);
            jsonWriter.WriteString("Description", cat.Description);
            jsonWriter.WriteNumber("Count", cat.Products.Count);

            jsonWriter.WriteStartArray("Products");
            foreach (var prod in cat.Products)
            {
                jsonWriter.WriteStartObject();

                jsonWriter.WriteNumber("Id", prod.ProductId);
                jsonWriter.WriteString("Name", prod.ProductName);
                jsonWriter.WriteNumber("Cost", prod.UnitPrice ?? 0);
                jsonWriter.WriteNumber("Stock", prod.UnitsInStock ?? 0);
                jsonWriter.WriteBoolean("Discontinued", prod.Discontinued);

                jsonWriter.WriteEndObject(); // prod
            }
            jsonWriter.WriteEndArray(); // products

            jsonWriter.WriteEndObject(); // cat
        }

        jsonWriter.WriteEndArray(); // categories
        jsonWriter.WriteEndObject(); 
    }
}

