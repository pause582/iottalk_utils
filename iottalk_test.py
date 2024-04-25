from iottalk_utils import iottalk_helper


if __name__ == '__main__':
    iottalk = iottalk_helper("0425","teststring","20240425")
    print(iottalk.register())
    for i in range(1):
        print(iottalk.push_twin('hello miku', 'hello luka'))
    res = iottalk.pull_twin()
    print(res)
