using Exercise_DataSerialization.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;

namespace Exercise_DataSerialization.Data;

public class NorthWindDb : DbContext
{
    public DbSet<Category> Categories { get; set; }
    public DbSet<Product> Products { get; set; }

    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        var config = new ConfigurationBuilder()
        .SetBasePath(AppContext.BaseDirectory)
        .AddJsonFile("AppSettings.json", optional: false, reloadOnChange: true)
        .Build();

        string? connectionString = config.GetConnectionString("NorthwindDB");

        if (string.IsNullOrEmpty(connectionString))
            Console.WriteLine("Error: Connection string is null or empty.");
        else
            Console.WriteLine($"Database Connection: {connectionString}");

        optionsBuilder.UseSqlServer(connectionString);
    }
}

