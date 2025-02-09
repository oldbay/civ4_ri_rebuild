
import functools


def monitor(func):
    """
    Monitor call Method of Class
    """
    @ functools.wraps(func)
    def wrapper(self, *args, **kwargs):
        ret = func(self, *args, **kwargs)

        print(
            "CALL: {}.{}\n    ARGS: {}\n    KWARGS: {}\n    RETURN: {}\n".format(
                self.__class__.__name__,
                func.__name__,
                args,
                kwargs,
                ret,
            )
        )

        return ret

    return wrapper
