using Exercise_DataSerialization.Data;
using Exercise_DataSerialization.Models;
using Microsoft.EntityFrameworkCore;

namespace Exercise_DataSerialization;

internal class Program
{
    static void Main(string[] args)
    {
        using NorthWindDb db = new();

        if (db.Categories == null)
        {
            Console.WriteLine("Error: The Categories table is not available.");
            return;
        }

        IQueryable<Category>? categories = db.Categories?.Include(c => c.Products);

        if (categories == null)
        {
            Console.WriteLine("Categories is empty.");
            return;
        }

        Serializers.XmlSerilizer(categories, false);
        Serializers.XmlSerilizer(categories, true);

        Serializers.JsonSerilizer(categories);

        Console.WriteLine($"Saved in directory: {Environment.CurrentDirectory}");
    }
}

