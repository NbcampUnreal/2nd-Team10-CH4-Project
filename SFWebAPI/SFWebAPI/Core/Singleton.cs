using System;

public abstract class Singleton<T> where T : class, new()
{
    public Singleton()
    {
        OnInit();
    }

    protected abstract void OnInit();

    private static readonly Lazy<T> instance = new Lazy<T>(() => new T());
    // private static readonly T instance = new T();

    public static T Instance { get { return instance.Value; } }
}