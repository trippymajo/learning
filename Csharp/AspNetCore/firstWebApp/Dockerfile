# AspNet
FROM mcr.microsoft.com/dotnet/aspnet:9.0 AS base
WORKDIR /app
EXPOSE 80

# .NET SDK
FROM mcr.microsoft.com/dotnet/sdk:9.0 AS build
WORKDIR /src
COPY MvcMovie/. .
RUN dotnet restore

#Debug
FROM build AS debug
RUN dotnet build -c Debug -o /app/build-debug

#Release
FROM build AS release
RUN dotnet build -c Release -o /app/build-release

#Publish
FROM build AS publish
RUN dotnet publish -c Release -o /app/publish

# Web Application
FROM debug AS final
WORKDIR /app
COPY --from=debug /app/build-debug /app
ENTRYPOINT ["dotnet", "MvcMovie.dll"]