# MVC pattern
Model - Describes a model for data. Consists of the Class and members with `{get; set;}`, which used to describe data in DB.  
View - Output to user. `*.cshtml` items. Some weird mix of html and cs.  
Controller - A bridge between Model and View, which make some manipulations with data and finally shows it to user via View.  
  
# Working with DBs
| Feature                    | ADO.NET | Dapper | Entity Framework Core |
|----------------------------|--------|--------|-----------------------|
| **Performance**            | 🔥 Fastest (low-level) | ⚡ Very Fast | 🐢 Moderate (ORM overhead) |
| **SQL Control**            | ✅ Full control | ✅ Full control | ❌ Limited (Generated SQL) |
| **Change Tracking**        | ❌ No | ❌ No | ✅ Yes |
| **Migrations**             | ❌ Manual | ❌ Manual | ✅ Automatic |
| **Bulk Operations**        | ✅ Best (SqlBulkCopy) | ⚡ Fast | ❌ Slow |
| **Transactions**           | ✅ Full control | ✅ Manual | ✅ Automatic |
| **Memory Usage**           | ✅ Optimized (Streams data) | ⚡ Low | ❌ High (Loads full objects) |
| **Ease of Use**            | ❌ Complex | ✅ Simple | ✅ Easiest |
| **Best For**               | High-performance apps | High-speed SQL apps | General applications |
  
| Feature          | SQL (Relational)          | NoSQL (Non-Relational)         |
|-----------------|--------------------------|--------------------------------|
| **Schema**      | Fixed, structured        | Dynamic, flexible             |
| **Data Model**  | Tables (rows & columns)  | Document, Key-Value, Graph, Column-Family |
| **Query Language** | SQL (Structured Query Language) | Varies (JSON, APIs, etc.) |
| **Transactions** | ACID-compliant          | BASE, eventual consistency    |
| **Scalability**  | Vertical scaling (more powerful servers) | Horizontal scaling (distributed nodes) |
| **Performance**  | Optimized for complex queries | Optimized for high-speed, large-scale data handling |
| **Use Case**    | Banking, ERP, e-commerce, structured data | Big data, real-time apps, IoT, flexible data storage |
  
## Entitty Framework Core
It is a object-to-data store mapping to be used alongside with DBs. Does not support with ahead-of-time(AOT) publishing.  
Application is build with dependency injections.  
Two ways to use:  
* **Database first**: DB exists, need to create a model. [Index()...] attribute not needed.
* **Code first**: DB not exists, need to create a model. [Index()...] attribute needed before the class.
  
**Automatic way**  
If Database is first:  
* dotnet-ef should be installed
* use such command in terminal `dotnet ef dbcontext scaffold ...`

Example:
```sh
dotnet ef dbcontext scaffold "Server=your_server;Database=your_db;User Id=your_user;Password=your_password;" Microsoft.EntityFrameworkCore.your_Db_Provider -o Models
```

As soon as Model created, can automatically generate methods around it which allow easy operations related with DB, also it will  
create the table as represented in model. For this do:  
1. Create Model
2. Controllers -> New Scaffolded Item... -> MVC -> MVC Controller with views, using Entity Framework (for example).  
Choose Model class, `+` for DB context & DB provider.
3. DB created using `Add-Migration InitialCreate` in NuGet Package Manager -> Package Manager Console.  
Here Migration files generated.
4. `Update-Database` - creates database.
  
> [!TIP]
> Scaffolding - using a tool to create classes that represent the model of exsisting database using reverse engineering.  
> The code by this tool is only approximate. So beware! 
  
**Manual way**  
1. Create Model
2. Add Data folder and {name}Context.cs class  
Which will inherit from DbContext and override `OnConfiguring()`
3. In Program.cs init the DB with new();
  
There are three loading patterns that are commonly used with EF Core:
* Eager loading: Load data early.  
* Lazy loading: Load data automatically just before it is needed.  
* Explicit loading: Load data manually.  
  
| Feature         | Eager Loading | Lazy Loading | Explicit Loading |
|---------------|--------------|-------------|----------------|
| **Data Loading Time** | Immediately with the main query | When accessed | Manually controlled |
| **Performance** | Efficient if related data is needed | Can cause extra queries (N+1 issue) | Efficient for selective loading |
| **Code Complexity** | Simple (`Include()`) | Requires proxies (`virtual`) | Requires manual `Load()` calls |
| **Best Use Case** | Always need related data | Rarely need related data | Need conditional loading |

```cs
// Eager:
var users = dbContext.Users
    .Include(u => u.Orders)  // Loads Orders along with Users
    .ToList();

// Lazy:
// In model
public virtual List<Order> Orders { get; set; }
// In code
var user = dbContext.Users.Find(1);
var orders = user.Orders; // Triggers query automatically

// Explicit:
var user = dbContext.Users.Find(1);
// Explicitly loading related data
dbContext.Entry(user).Collection(u => u.Orders).Load();
```
  
### Dealing with relationships
Foreign keys means relations in SQL, in order to implement them in C#.  
Relation showing lanes are virtual in order to allow EF features as lazy loading and etc.  
```cs
public class Category
{
    public int CategoryId { get; set; }
    public string CategoryName { get; set; } = null!;

    // One Category has many Products (1:N)
    // Allow devs to add products to the category, also avoids exception if Count called
    public virtual ICollection<Product> Products { get; set; } = new HashSet<Product>();
}

public class Product
{
    public int ProductId { get; set; }
    public string ProductName { get; set; } = null!;
    
    // Many Products belong to one Category (N:1)
    // Category should be not null to be used in Product with CategoryId.
    public int CategoryId { get; set; } 
    public virtual Category Category { get; set; } = null!;
}
```
  
### CRUD operations
1. Create  
`Add()` and `SaveChanges()` to insert new records.
2. Read
* All Records `var users = context.Users.ToList();`
* Single Record `var user = context.Users.Find(1);`
* Filtering `var user = context.Users.FirstOrDefault(u => u.Email == "mail@example.com");`
* Eager Loading (Include Related Data) `var usersWithOrders = context.Users.Include(u => u.Orders).ToList();`
3. Update
* By fetching:
```cs
var user = context.Users.Find(1);
if (user != null)
{
    user.Name = "Alice Updated";
    context.SaveChanges();
}
```
* Without fetching (Directly):
```cs
var user = new User { Id = 1, Name = "Updated Name" };
context.Users.Attach(user);
context.Entry(user).Property(u => u.Name).IsModified = true;
context.SaveChanges();
```
4. Delete
* By fetching:
```cs
var user = context.Users.Find(1);
if (user != null)
{
    context.Users.Remove(user);
    context.SaveChanges();
}
```
* Without fetching (Directly):
```cs
var user = new User { Id = 1 };
context.Users.Attach(user);
context.Users.Remove(user);
context.SaveChanges();
```
  
>[!TIP]
> .AsNoTracking() for read-only queries to improve performance.  
> For batch updates, use raw SQL or EF Core’s ExecuteUpdate() (EF Core 7+).  
> Use ExecuteDelete() for bulk deletions when using EF Core 7+.  
  
### Pattern matching with `.Like()`
While LINQ does not support Like (replaced with other methods), EF can doi it.  
```cs
string input = "CAT"
var results = dbContext.Users
    .Where(u => EF.Functions.Like(u.Name, $"%{input}%"))
    .ToList();
```
"A%" → Starts with "A"  
"%A" → Ends with "A"  
"%A%" → Contains "A"  
  
### Using SQL query directly
`.FromSql()` EF Core 7 or `FromSqlInterpolated()` for EF core 6 and earlier.  
It can only be called on a `DbSet<T>` not on query.
  
### Using LINQ
Allows to work with DB. When query is enumerated with `foreach` or calling method like `ToArray` or `ToList` it will trigger the execution of query agains DB.  
`Where`,`Select`,`HroupBy`,`SelectMany`,`OfType`,`OrderBy`,`ThenBy`,`Join`,`GroupJoin`,`Take`,`Skip`,`Reverse` - safe for not triggering execution agains DB.  
To know in which SQL query LINQ converts code can be done by `something.ToQueryString()`.
Also it is possible to tag query for logging. With `.TagWith("Something")`
```cs
using MyDB db = new();

// Building a query
IQueryable<Category>? categories = db.Categories?.Include(c => c.Products);

// Executing the query
if (categories != null || !categories.Any())
{
    foreach (Category c in categories) // Triggers DB query execution
        WriteLine($"{c.CategoryName} has {c.Products.Count} products");
}
```
  
Also EF Core 5 suports filtered includes like:
```cs
IQueryable<Category>? categories = db.Categories?
    .Include(c => c.Products.Where(p => p.Stock >= 5));
```
  
**Getting Single entity**  
The queries will be immediately converted and executed.  
`.First` - Can match one or more entities and only first will be returned. `LIMIT 1`  
`.Single` - Matches exactly one entity and returned. Else if there is more then one match - exception. `LIMIT 2`  

# Middleware
It is like a queue of what needs to be done. 
```cs
public async Task Invoke(HttpContext context) //<- This is midleware
{
    Console.WriteLine("A");
    await _next(context); //<- This is calling next middleware
    Console.WriteLine("B");
}
```

# Dependency injection (DI)
Dependency Injection (DI) is a built-in feature of ASP.NET Core that helps manage dependencies in a clean and testable way.  
Instead of creating objects manually, ASP.NET Core injects them automatically where needed. (You can do manually injection if working with ADO.NET)  
In C++ we do it all manually via Design patterns like fabric, And passing params in constructors.  

| Lifetime       | Description | Example Usage |
|--------------|-------------|--------------|
| **Singleton** | One instance for the entire application lifetime. | Logging, Configuration, Caching |
| **Scoped** | One instance per request (useful in web apps). | Database Context, Business Logic Services |
| **Transient** | A new instance every time it is requested. | Lightweight, short-lived services |
  
## How to do DI
1. Register Services in Program.cs
```cs
var builder = WebApplication.CreateBuilder(args);
builder.Services.AddScoped<IUserService, UserService>();  // Register..
var app = builder.Build();
app.Run();
```
  
2. Inject Dependencies into a Controller or Service
```cs
public class UserController : ControllerBase
{
    private readonly IUserService _userService;

    public UserController(IUserService userService)
    {
        _userService = userService;
    }

    public IActionResult GetUsers()
    {
        var users = _userService.GetAllUsers();
        return Ok(users);
    }
}
```

# Vulnerabilities
## Over-posting
Overposting happens when an attacker or a user submits more data than expected (e.g., modifying fields they shouldn't).  
```cs
[HttpPost]
[ValidateAntiForgeryToken]
public async Task<IActionResult> Create(Movie movie)
{
    _context.Add(movie);  // Directly binding the entire entity (BAD)
    await _context.SaveChangesAsync();
    return RedirectToAction(nameof(Index));
}
```
  
**Defend with:**
  
### ViewModel class
```cs
public class MovieCreateViewModel
{
    public string Title { get; set; }
    public string Genre { get; set; }
    public int Year { get; set; }
}
```
  
```cs
[HttpPost]
[ValidateAntiForgeryToken]
public async Task<IActionResult> Create(MovieCreateViewModel model)
{
    if (ModelState.IsValid)
    {
        var movie = new Movie
        {
            Title = model.Title,
            Genre = model.Genre,
            Year = model.Year
        };

        _context.Add(movie);
        await _context.SaveChangesAsync();
        return RedirectToAction(nameof(Index));
    }
    return View(model);
}
```
  
### Bind attribute
```cs
[HttpPost]
[ValidateAntiForgeryToken]
public async Task<IActionResult> Create([Bind("Title,Genre,Year")] Movie movie)
{
    if (ModelState.IsValid)
    {
        _context.Add(movie);
        await _context.SaveChangesAsync();
        return RedirectToAction(nameof(Index));
    }
    return View(movie);
}
```
  
### From... attribute
[FromForm] - Accepts data from form submissions (e.g., HTML forms).
[FromBody] - Accepts data from the request body (useful for APIs).
[FromQuery] - Accepts data from URL query parameters.
  
```cs
[HttpPost]
public IActionResult Create([FromForm] MovieCreateViewModel model)
{
    if (ModelState.IsValid)
    {
        var movie = new Movie { Title = model.Title, Genre = model.Genre, Year = model.Year };
        _context.Movies.Add(movie);
        _context.SaveChanges();
        return RedirectToAction("Index");
    }
    return View(model);
}
```
  
### JsonIgnore attribute
```cs
public class Movie
{
    public int Id { get; set; }
    public string Title { get; set; }
    public string Genre { get; set; }
    
    [JsonIgnore] // Prevent overposting of sensitive fields
    public bool IsAdminOnly { get; set; }
}
```
  
## Cross-Site Request Forgery (CSRF/XSRF)
CSRF (Cross-Site Request Forgery) is a security vulnerability where an attacker tricks a logged-in user into performing unwanted actions on a trusted website without their consent.  
**Example Attack Scenario:**  
1. A user logs into yourbank.com.
2. The attacker sends a malicious request (e.g., a hidden form submission) to yourbank.com/transfer without user interaction.
3. Since the user's session (cookies) is valid, the request executes with their permissions.
  
**Defend with:**
  
### Anti-Forgery Tokens
```html
// Razor page
<form asp-action="Edit">
    @Html.AntiForgeryToken()

// Or
<form asp-action="Edit" method="post" asp-antiforgery="true">
```
  
```cs
// Controller
[HttpPost]
[ValidateAntiForgeryToken] // Ensures anti-forgery token is required
public async Task<IActionResult> Edit(int id, [Bind("Id,Title,ReleaseDate,Genre,Price")] Movie movie)
```
  
