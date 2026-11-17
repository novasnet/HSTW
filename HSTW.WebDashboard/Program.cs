using HSTW.WebDashboard.Services;

var builder = WebApplication.CreateBuilder(args);

// 1. Servicios estándar de Razor Pages y Blazor Server
builder.Services.AddRazorPages();
builder.Services.AddServerSideBlazor();

// 2. Registrar tu motor tensorial C++ como Singleton único
builder.Services.AddSingleton<TensorEngineService>();

var app = builder.Build();

// 3. Configurar el pipeline HTTP
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();
app.UseRouting();

app.MapBlazorHub();
app.MapFallbackToPage("/_Host");

app.Run();